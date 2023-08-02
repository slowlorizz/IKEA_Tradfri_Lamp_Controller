#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#include "lib/Wlan.hpp"
#include "lib/Remote.hpp"
#include "lib/API.hpp"
 
void setup(void) {
  Remote::register_pins();
  Wlan::connect();
  MDNS.begin("esp8266");
  API::init();
}
 
void loop(void) {
  API::handle_requests();
}