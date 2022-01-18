/* 
 Automatic Telescope Cover
 Board: NodeMCU-32S, Motor: LX-224 HV
 Author: Jamie Chang 2022/01/18
 Contact: jamiechang917@gmail.com 
 */
// Libraries
#include <WiFi.h>
#include <WebServer.h>
#include "WebPages.h"

// Wifi config
char wifi_ssid[] = "Wooster";
char wifi_passwd[] = "123456789";

// Declear parameters
WebServer server(80);

bool connectWifi() {
  Serial.println("======WiFi Connection======");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); delay(100);
  Serial.print("Start to connect "); Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_passwd); delay(15000);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi is connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("========================="); Serial.println("");
    return true;
  }
  Serial.println("Wifi connection failed");
  Serial.println("========================="); Serial.println("");
  return false;
}

void handleNotFound() {
  server.send(404, "text/plain","404 Not Found");
}

void createWebServer() {
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Webserver is running...");
}

void setup() {
  Serial.begin(115200);
  while (!connectWifi()) {}
  createWebServer();
  
}

void loop() {
  server.handleClient();

}
