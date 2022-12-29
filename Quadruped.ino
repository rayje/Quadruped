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
  server.on("/moveForward", forward);

  server.begin();
  Serial.println("HTTP server started");
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
  double x = server.arg("x").toDouble();
  double z = server.arg("z").toDouble();

  Serial.printf(" - leg: %d\n", legIndex);
  Serial.printf(" - x: %f z: %f\n", x, z);

  bot.moveServoTo(legIndex, x, z);
  handleRoot();
}

void forward() {
  bot.forward(server.arg("steps").toInt(), server.arg("delay").toInt());
  bot.reset();
  handleRoot();
}