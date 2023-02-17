#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
 
const char* ssid = "Vaccine-Chip-Connection-Point#24";
const char* password = "Y0uSh4llP4ss!";
IPAddress staticIP(192, 168, 123, 66); //ESP static ip
IPAddress gateway(192, 168, 123, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS
const char* deviceName = "esp_wifi_01.loc";
 
ESP8266WebServer server(80);

namespace REMOTE {
  int onOff_gpio = 12; // D6
  int brighter_gpio = 14; // D5
  int darker_gpio = 4; // D2
  int colorLeft_gpio = 5; // D1
  int colorRight_gpio = 2; // D4
  int PHASE_TIME = 500;
  int COLOR_SHIFT_DELAY = 1250;

  void init_pins() {
    pinMode(REMOTE::onOff_gpio, OUTPUT);
    pinMode(REMOTE::brighter_gpio, OUTPUT);
    pinMode(REMOTE::darker_gpio, OUTPUT);
    pinMode(REMOTE::colorLeft_gpio, OUTPUT);
    pinMode(REMOTE::colorRight_gpio, OUTPUT);
  }

  void press_button(int button_gpio){
    digitalWrite(button_gpio, LOW);
    delay(REMOTE::PHASE_TIME);
    digitalWrite(button_gpio, HIGH);
  }

  void press_color_button(int button_gpio){
    digitalWrite(button_gpio, LOW);
    delay(REMOTE::COLOR_SHIFT_DELAY);
    digitalWrite(button_gpio, HIGH);
  }

  void toggle_power(){
    REMOTE::press_button(REMOTE::onOff_gpio);
  }

  void brighter() {
    REMOTE::press_button(REMOTE::brighter_gpio);
  }

  void darker() {
    REMOTE::press_button(REMOTE::darker_gpio);
  }

  void color_left() {
    REMOTE::press_color_button(REMOTE::colorLeft_gpio);
  }

  void color_right() {
    REMOTE::press_color_button(REMOTE::colorRight_gpio);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggle_powerstate(){
  if (server.method() == HTTP_POST) {
    REMOTE::toggle_power();
 
    DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    String buf;
    serializeJson(doc, buf);
    server.send(201, F("application/json"), buf);
  }
}

void set_brighter(){
  if (server.method() == HTTP_POST) {
    REMOTE::brighter();
 
    DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    String buf;
    serializeJson(doc, buf);
    server.send(201, F("application/json"), buf);
  }
}

void set_darker(){
  if (server.method() == HTTP_POST) {
    REMOTE::darker();
 
    DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    String buf;
    serializeJson(doc, buf);
    server.send(201, F("application/json"), buf);
  }
}

void shift_color_left(){
  if (server.method() == HTTP_POST) {
    REMOTE::color_left();
 
    DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    String buf;
    serializeJson(doc, buf);
    server.send(201, F("application/json"), buf);
  }
}

void shift_color_right(){
  if (server.method() == HTTP_POST) {
    REMOTE::color_right();
 
    DynamicJsonDocument doc(512);
    doc["status"] = "OK";
    String buf;
    serializeJson(doc, buf);
    server.send(201, F("application/json"), buf);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// Define routing
void restServerRouting() {
    server.on("/power", HTTP_POST, toggle_powerstate);
    server.on("/brighter", HTTP_POST, set_brighter);
    server.on("/darker", HTTP_POST, set_darker);
    server.on("/color/left", HTTP_POST, shift_color_left);
    server.on("/color/right", HTTP_POST, shift_color_right);
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

 
void setup(void) {
  Serial.begin(9600);

  REMOTE::init_pins();

  WiFi.disconnect();
  WiFi.hostname(deviceName);
  WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("\n");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
  server.handleClient();
}