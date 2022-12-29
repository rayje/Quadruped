#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Quadbot.h"
#include "WebPage.h"


// Set WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASS ""

WebPage webpage;
Quadbot bot;
ESP8266WebServer server(80);
int minMicro = 544;
int maxMicro = 2400;

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
  server.on("/", handleRoot);
  server.on("/reset", reset);
  server.on("/moveServo", moveServoTo);
  server.on("/moveMax", moveMax);
  server.on("/moveMin", moveMin);
  server.on("/moveLegMax", moveLegMax);
  server.on("/moveLegMin", moveLegMin);
  server.on("/moveForward", forward);
  server.on("/moveMicro", moveMicro);

  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot() {
  String html = webpage.getRootPage(bot);
  server.send(200, "text/html", html);
}

void reset() {
  bot.reset();
  handleRoot();
}

void moveServoTo() {
  int legIndex = server.arg("legIndex").toInt();
  int x = server.arg("x").toInt();
  int y = server.arg("y").toInt();
  int z = server.arg("z").toInt();

  Serial.print(" - leg: ");
  Serial.println(legIndex);
  Serial.print(" - x: "); 
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.println(z);

  bot.moveServoTo(legIndex, x, y, z);
  handleRoot();  
}

void moveMax() {
  bot.moveServoMax();
  handleRoot();
}

void moveMin() {
  bot.moveServoMin();
  handleRoot();
}

void moveLegMax() {
  bot.moveLegMax();
  handleRoot();
}

void moveLegMin() {
  bot.moveLegMin();
  handleRoot();
}

void forward() {
  bot.forward(server.arg("steps").toInt(), server.arg("delay").toInt());
  bot.reset();
  handleRoot();
}

void moveMicro() {
  bot.moveMicro(server.arg("servo").toInt(), server.arg("microSeconds").toInt());
  handleRoot();
}