/*

  WiFi UDP Send Byte on Button Press
  Based on UDP Send and Receive String

  created 3 February 2019
  by Michael Shiloh
  changed 18 February 2019
  by Magda and Sara
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

const int rButton = 5;
const int bButton = 4;
const int gButton = 3;
boolean rButtonState;
boolean bButtonState;
boolean gButtonState;
boolean lastRState = LOW;
boolean lastBState = LOW;
boolean lastGState = LOW;

int GreenLED = 9;
int BlueLED = 8;
int RedLED = 7;

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

  pinMode(RedLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
}

void loop() {

  sendCmnd();
  receiveCmnd();

}

void sendCmnd() {
  rButtonState = digitalRead(rButton);
  bButtonState = digitalRead(bButton);
  gButtonState = digitalRead(gButton);

  if (rButtonState != lastRState || bButtonState != lastBState || gButtonState != lastGState) {

    Serial.println("Button state changed; sending new state");
    Udp.beginPacket(receivingDeviceAddress, receivingDevicePort);
    sendBuffer[0] = rButtonState;
    sendBuffer[1] = bButtonState;
    sendBuffer[2] = gButtonState;
    Udp.write(sendBuffer, 3);
    Udp.endPacket();
    
    lastRState = rButtonState;
    lastBState = bButtonState;
    lastGState = gButtonState;
  }
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

    // We are expecting exactly one byte,
    // and that byte tells us whether to turn
    // the LED on or off. There is no need to
    // inspect the byte, simply send it directly
    // to digitalWrite()
    boolean red = packetBuffer[0];
    Serial.println(red);
    digitalWrite(RedLED, red);
    boolean blue = packetBuffer[1];
    Serial.println(blue);
    digitalWrite(BlueLED, blue);
    boolean green = packetBuffer[2];
    Serial.println(green);
    digitalWrite(GreenLED, green);

    // if we wanted to send anything back e.g. to
    // acknowledge the packet
    // this would be the place

  }
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
