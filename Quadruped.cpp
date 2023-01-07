#include <Adruino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Quadbot.h"
#include "WebPage.h"
#include "Types.h"


// Set WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASS ""

WebPage webpage;
Quadbot bot;
ESP8266WebServer server(80);
int minMicro = 544;
int maxMicro = 2400;

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
  server.on("/", showRoot);
  server.on("/reset", reset);
  server.on("/moveServo", moveServoTo);
  server.on("/moveForward", forward);
  server.on("/setServoAngle", setServoAngle);
  server.on("/moveArmRange", moveArmRange);
  server.on("/moveArmOne", moveArmOne);

  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot(String html) {
  server.send(200, "text/html", html);
}

void showRoot() {
  handleRoot(webpage.getRootPage(bot));
}

void reset() {
  bot.reset();
  handleRoot(webpage.getRootPage(bot));
}

void moveServoTo() {
  int legIndex = server.arg("legIndex").toInt();
  double x = server.arg("x").toDouble();
  double z = server.arg("z").toDouble();

  Serial.printf(" - leg: %d\n", legIndex);
  Serial.printf(" - x: %f z: %f\n", x, z);

  Indicies idx = bot.moveServoTo(legIndex, x, z);
  handleRoot(webpage.handleServoMoved(bot, idx));
}

void setServoAngle() {
  int index = server.arg("index").toInt();
  int angleValue = server.arg("angle").toInt();

  ServoAngle angle = {index, angleValue};
  bot.setServoAngle(angle);

  handleRoot(webpage.handleSetServoAngle(bot, angle));
}

void forward() {
  bot.forward(server.arg("steps").toInt(), server.arg("delay").toInt());
  bot.reset();
  handleRoot(webpage.getRootPage(bot));
}

void moveArmRange() {
  bot.moveArms();
  handleRoot(webpage.getRootPage(bot));
}

void moveArmOne() {
  bot.moveArmOne();
  handleRoot(webpage.getRootPage(bot));
}

void setup() {
  connectWifi();

  serverSetup();

  bot.setup();
}

void loop() {
  server.handleClient();
}
