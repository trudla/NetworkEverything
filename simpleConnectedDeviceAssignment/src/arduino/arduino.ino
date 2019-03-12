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

char packetBuffer[255]; //buffer to hold incoming packet
char sendBuffer[255];

WiFiUDP Udp;

const int rButton = A1;
const int bButton = A2;
const int gButton = A3;
boolean rButtonState;
boolean bButtonState;
boolean gButtonState;

// IP address of the receiving device
IPAddress receivingDeviceAddress(192, 168, 1, 25);
unsigned int receivingDevicePort = 2390;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
/*
 * I was thinking of either sending data all the time or only when prompted but decided to send it all the time
 */
  sendCmnd();
  delay(1000);

}

void sendCmnd() {
/*
 * Buttons can also represent different moisture sensors
 */
  
  rButtonState = analogRead(rButton);
  bButtonState = analogRead(bButton);
  gButtonState = analogRead(gButton);

  //if (rButtonState != lastRState || bButtonState != lastBState || gButtonState != lastGState) {

    Serial.println("Button state changed; sending new state");
    Udp.beginPacket(receivingDeviceAddress, receivingDevicePort);
    
    sendBuffer[0] = map(rButtonState,800,200,0,100); //DUMMY VALUES!!!
    sendBuffer[1] = map(bButtonState,800,200,0,100);
    sendBuffer[2] = map(gButtonState,800,200,0,100);
    Udp.write(sendBuffer, 3);
    Udp.endPacket();
   /* 
    lastRState = rButtonState;
    lastBState = bButtonState;
    lastGState = gButtonState;
  } */
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
