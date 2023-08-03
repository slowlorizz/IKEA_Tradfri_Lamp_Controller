#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#include "lib/Wlan.hpp"
#include "lib/Remote.hpp"
#include "lib/API.hpp"

// brightness levels: 5
// colors: 9 (0 included) --> range: 0 - 8

// http://192.168.0.211/set?power=on&brightness=3&color=1

void setup(void) {
  Remote::register_pins();
  Wlan::connect();
  MDNS.begin("esp8266");
  API::init();

  Remote::wakeup_pins();

  Serial.begin(115200);
}
 
void loop(void) {
  API::handle_requests();
}