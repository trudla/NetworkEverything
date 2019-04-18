/* 
This is the websocket library which will allow us to send messages
back to the web server 
*/
var socket = io();
var selectedVal;

function getMoisture() {
  console.log ("sending a message to the web server to get the moisture level");
  // this is how we send a message back to the web server
  socket.emit('getMoisture'); 
};

function GetSelectedItem(perct)
{
    var p = document.getElementById(perct);
    console.log(p.options[p.selectedIndex].value);
    selectedVal = p.options[p.selectedIndex].value;
    data_first.style.display = 'none';
    actMLvl.style.display='block';
    data_second.style.display = 'block';
    document.body.style.setProperty("--main-bg", "#f4e842");
}

function changeBackground(color) {
   document.body.style.background = color;
}

socket.on('reading', function(values) {
	var data = values;
	document.getElementById("reading").innerHTML = data;
	document.getElementById("perc_sign").innerHTML = "%";
	//document.location.reload(false);
	console.log(data);
	console.log(selectedVal);
	decision.style.display = 'block';
	if (data > selectedVal){
		document.body.style.setProperty("--main-bg", "#84f94d");
		document.getElementById("decision").innerHTML = "No need to water the plant YET";
	}
	else {
		document.body.style.setProperty("--main-bg", "#f44141");
		document.getElementById("decision").innerHTML = "Water your plant if you don't want to kill it";
	}
});
