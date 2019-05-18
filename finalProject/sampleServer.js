

/* UDP server talks to Arduino */
var dgram = require('dgram');
var udpServer = dgram.createSocket('udp4')
var LAPTOP_UDP_PORT = 2390;
var RASPI_LISTENING_AT= 5000; ///won't be using this
var RASPI_ADDRESS = '10.225.25.121'; //or this or maybe yes 

/* HTTP server talks to browser */
const http = require('http')
const express = require('express'); // web server application
const app = express();        // instantiate express server
const httpServer = http.Server(app);  // connects http library to server
const HTTP_SERVER_PORT = 8000; 

// Express creates the simple web page
// The argument says where to find pages and scripts
app.use(express.static('public'));  

// websockets so that webpage can talk back to server
const webSocket = require('socket.io')(httpServer);  

//send to website
var recMsg;

/* UDP server callback functions */

function UDPServerIsListening() {
	console.log('UDP Server is listening');
}

function UDPServerReceivedMessage(message, sender) {

	// print the message
	recMsg = message.toString().replace("b'", "").replace("'",""); //raspi sends data as b'data'
	console.log(
		'Received message from: ' +
		sender.address + 
		':' + 
		sender.port);
	console.log(
		'Message length: ' +
		message.length + 
		' Message contents: ' +recMsg);
  	webSocket.emit('reading', recMsg);
}

/* Register the UDP callback functions */
udpServer.bind(LAPTOP_UDP_PORT );
udpServer.on('listening', UDPServerIsListening);
udpServer.on('message', UDPServerReceivedMessage);

/* HTTP callback functions */

httpServer.listen(HTTP_SERVER_PORT, () => {
	console.log('httpServer: Listening at', httpServer.address());
});

httpServer.on('connection', () => {
  console.log("httpServer: An HTTP client has connected")
});

/* and here is the websocket event handler */
webSocket.on('connect', (socket) => {
	console.log('Web server socket: Client connected');
});