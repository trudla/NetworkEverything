/*

  WiFi UDP Send Byte on Button Press
  Based on UDP Send and Receive String

  created 3 February 2019
  by Michael Shiloh
  changed 4 February 2019
  by Magda and Sara
  changed 4 March 2019
  by Magda
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 5000;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char sendBuffer[255];

WiFiUDP Udp;

const int sensor = A1;
const int curr = 5;
const int redLED = 7;
const int greenLED = 9;
int moisture = 0;
int received = 0;

// IP address of the receiving device
IPAddress receivingDeviceAddress(192, 168, 1, 33);
unsigned int receivingDevicePort = 2390;

void setup() {
  pinMode(sensor, OUTPUT);
  pinMode(curr, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:p
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus(); //function written below loop

  Serial.print("Initializing WiFiUDP library and listening on port ");
  Serial.println(localPort);
  Udp.begin(localPort);

}

void loop() {
  receiveCmnd();
}

void receiveCmnd(){
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  // if nobody sent us anything, packetSize
  // would be zero, so a non-zero packetSize
  // means we have something to work on
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    
    if (packetBuffer[0] == '1')
    {
       digitalWrite(curr, HIGH);
       reading();
       sending();
       digitalWrite(curr, LOW);
    }
    else if (packetBuffer[0] == 'r')
    {
      Serial.print("Received: ");
      Serial.println(packetBuffer[0]);
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
    }
    else if (packetBuffer[0] == 'g')
    {
      Serial.print("Received: ");
      Serial.println(packetBuffer[0]);
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
    }
    else if (packetBuffer[0] == 'n')
    {
      Serial.print("Received: ");
      Serial.println(packetBuffer[0]);
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
    }
    else 
    {
       Serial.print("Received value is not 1 but: ");
       Serial.println(packetBuffer);  
    }  
    
    // if we wanted to send anything back e.g. to
    // acknowledge the packet
    // this would be the place
    
  }
}  

void sending() {
    sendBuffer[0] = moisture;
    Udp.beginPacket(receivingDeviceAddress, receivingDevicePort);
    Serial.print("Sending: ");
    Serial.println(sendBuffer[0]);
    Udp.write(sendBuffer, 1);  //can add more data for comparison
    Udp.endPacket();
}

void reading (){
  moisture = analogRead(sensor);
  moisture = map(moisture, 0, 870, 0, 100);
  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("My IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
