#ifndef WLAN_H
#define WLAN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "WifiSecrets.hpp"

class Wlan
{
    private:

    public:
        inline static void connect() {
            IPAddress staticIP(192, 168, 0, 211);
            IPAddress subnet(255,255,255,0);
            IPAddress gateway(192,168,0,1);
            IPAddress dns(8,8,8,8);
            char* deviceName = "tradfri_lamp_controller.loc";

            WiFi.disconnect();
            WiFi.hostname(deviceName);
            WiFi.config(staticIP, subnet, gateway, dns);
            WiFi.mode(WIFI_STA);
            WiFi.begin(WifiSecrets::WifiSSID, WifiSecrets::WifiPassword);
            
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
            }
        }
};

#endif //WLAN_H
