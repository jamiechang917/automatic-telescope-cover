/* 
 Automatic Telescope Cover
 Board: NodeMCU-32S, Motor: SG-90
 Author: Jamie Chang 2022/01/18
 Contact: jamiechang917@gmail.com 
 WebServer.h: https://github.com/zhouhan0126/WebServer-esp32.git
 */
// Libraries
//#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "index.h"

// Wifi config
#define WIFI_SSID "WFH"
#define WIFI_PASSWD "27661005@Calvin3171"
df
// Declear parameters
WebServer server(80);
Servo motor;
int flat_panel_pin = 25; int motor_pin = 33;
int cover_state, flat_panel_state = 0; // 0 is close, 1 is open.
float motor_angle = 90, motor_temp = 0;
String local_ip;
char XML[2048]; // data in xml format.

bool connectWifi() {
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); delay(100);
  Serial.println("======WiFi Connection======");
  Serial.printf("Start to connect %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD); delay(15000);
  digitalWrite(LED_BUILTIN, LOW);

  if (WiFi.status() == WL_CONNECTED) {
    local_ip = WiFi.localIP().toString();
    Serial.println("Wifi is connected");
    Serial.printf("Local IP: %s\n", local_ip.c_str());
    Serial.println("========================="); Serial.println("");
    return true;
  }
  Serial.println("Wifi connection failed");
  Serial.println("========================="); Serial.println("");
  return false;
}

void handleXML() { // pass the data to the web page
  char buf[64];
  strcpy(XML, "<?xml version = \"1.0\" encoding=\"UTF-8\"?>\n<Data>\n");

  // wifi_ssid
  sprintf(buf, "<wifi_ssid>%s</wifi_ssid>\n", WIFI_SSID);
  strcat(XML, buf);
  // local_ip
  sprintf(buf, "<local_ip>%s</local_ip>\n", local_ip.c_str());
  strcat(XML, buf);
  // cover_state
  sprintf(buf, "<cover_state>%d</cover_state>\n", cover_state);
  strcat(XML, buf);
  // flat_panel_state
  sprintf(buf, "<flat_panel_state>%d</flat_panel_state>\n", flat_panel_state);
  strcat(XML, buf);
  // motor_angle
  sprintf(buf, "<motor_angle>%.2f</motor_angle>\n", motor_angle);
  strcat(XML, buf);
  // motor_temp
  sprintf(buf, "<motor_temp>%.2f</motor_temp>\n", motor_temp);
  strcat(XML, buf);

  strcat(XML, "</Data>\n");
  server.send(200, "text/xml", XML);
//  Serial.println(XML);
}

void handleHomePage() {
  server.send(200, "text/html", main_page);
}

void handleNotFound() {
  server.send(404, "text/plain","404 Not Found");
}

void handleOpenCover() {
  if (cover_state == 0) {
    motor.attach(motor_pin);
    motor.write(0);
    delay(2000);
//    motor.detach();
    motor_angle = 0;
    cover_state = 1;
  }
  server.send(200, "text/plain", "");
  Serial.println("Open the cover");
}

void handleCloseCover() {
  if (cover_state == 1) {
    motor.attach(motor_pin);
    motor.write(110);
    delay(2000);
//    motor.detach();
    motor_angle = 90;
    cover_state = 0;
  }
  server.send(200, "text/plain", "");
  Serial.println("Close the cover");
}

void handleOpenFlatPanel() {
  if (flat_panel_state == 0) {
    flat_panel_state = 1;
  }
  server.send(200, "text/plain", "");
  Serial.println("Open the flat panel");
}

void handleCloseFlatPanel() {
  if (flat_panel_state == 1) {
    flat_panel_state = 0;
  }
  server.send(200, "text/plain", "");
  Serial.println("Close the flat panel");
}

void createWebServer() {
  server.on("/", handleHomePage);
  server.on("/xml", handleXML);
  server.onNotFound(handleNotFound);
  server.on("/open_cover", handleOpenCover);
  server.on("/close_cover", handleCloseCover);
  server.on("/open_flat_panel", handleOpenFlatPanel);
  server.on("/close_flat_panel", handleCloseFlatPanel);
  server.begin();
  Serial.println("Webserver is running...");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(flat_panel_pin, OUTPUT);
  motor.attach(motor_pin);
  motor.write(110); // close the cover
  delay(3000);
  motor.detach();
  Serial.begin(115200);
  while (!connectWifi()) {}
  createWebServer();
}

void loop() {
  server.handleClient();
  digitalWrite(flat_panel_pin, flat_panel_state);
}
