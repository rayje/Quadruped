#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Quadbot.h"


// Set WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASS ""

Quadbot bot;
ESP8266WebServer server(80);

String webPage = "\
<form action=\"/servo_up\" method=\"get\" id=\"form1\"></form>\
<button type=\"submit\" form=\"form1\" value=\"Up\">Up</button>\
<form action=\"/servo_down\" method=\"get\" id=\"form2\"></form>\
<button type=\"submit\" form=\"form2\" value=\"Down\">Down</button>\
<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform\">\
<input type=\"text\" name=\"servo\" value=\"0\"><br>\
<input type=\"text\" name=\"position\" value=\"120\"><br>\
<input type=\"submit\" value=\"Submit\">\
</form>";

void setup() {
  connectWifi();

  serverSetup();

  bot.setup();
}

void loop() {
  server.handleClient();
}

void connectWifi() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  IPAddress ip(192, 168, 50, 161);
  IPAddress gateway(192, 168, 50, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void serverSetup() {
  server.on ( "/", handleRoot);
  server.on("/servo_up", servoUp);
  server.on("/servo_down", servoDown);
  server.on("/postform", moveServoTo);

  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot() {
  server.send(200, "text/html", webPage);
}

void servoUp() {
  bot.servoUp();
  Serial.println("ServoUp");
  handleRoot();
}

void servoDown() {
  bot.servoDown(); 
  Serial.println("ServoDown: " + server.arg(""));
  handleRoot();
}

void moveServoTo() {
  bot.moveServoTo(server.arg("servo").toInt(), server.arg("position").toInt());
  Serial.println(" - servo: " + server.arg("servo"));
  Serial.println(" - pos: " + server.arg("position"));
  handleRoot();  
}
