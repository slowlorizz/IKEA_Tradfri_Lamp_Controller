#ifndef API_H
#define API_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "Remote.hpp"

ESP8266WebServer WebServer(80);

class API
{
    private:
        inline static void post_success_answer(){
            DynamicJsonDocument doc(512);
            doc["status"] = "OK";
            String buf;
            serializeJson(doc, buf);
            WebServer.send(201, F("application/json"), buf);
        }

        inline static void post_power_route(){
            if(WebServer.method() == HTTP_POST) {
                Remote::toggle_power();
                API::post_success_answer();
            }
        }

        inline static void post_brighter_route(){
            if(WebServer.method() == HTTP_POST) {
                Remote::brighter();
                API::post_success_answer();
            }
        }

        inline static void post_darker_route(){
            if(WebServer.method() == HTTP_POST) {
                Remote::darker();
                API::post_success_answer();
            }
        }

        inline static void post_left_route(){
            if(WebServer.method() == HTTP_POST) {
                Remote::color_left();
                API::post_success_answer();
            }
        }

        inline static void post_right_route(){
            if(WebServer.method() == HTTP_POST) {
                Remote::color_right();
                API::post_success_answer();
            }
        }

    public:
        inline static void set_routes(){
            WebServer.on("/power", HTTP_POST, API::post_power_route);
            WebServer.on("/brighter", HTTP_POST, API::post_brighter_route);
            WebServer.on("/darker", HTTP_POST, API::post_darker_route);
            WebServer.on("/color/left", HTTP_POST, API::post_left_route);
            WebServer.on("/color/right", HTTP_POST, API::post_right_route);
        }

        inline static void notFound_handler() {
            String message = "File Not Found\n\n";
            message += "URI: ";
            message += WebServer.uri();
            message += "\nMethod: ";
            message += (WebServer.method() == HTTP_GET) ? "GET" : "POST";
            message += "\nArguments: ";
            message += WebServer.args();
            message += "\n";
            for (uint8_t i = 0; i < WebServer.args(); i++) {
                message += " " + WebServer.argName(i) + ": " + WebServer.arg(i) + "\n";
            }
            WebServer.send(404, "text/plain", message);
        }

        inline static void init() {
            API::set_routes();
            WebServer.onNotFound(API::notFound_handler);
            WebServer.begin();
        }

        inline static void handle_requests(){
            WebServer.handleClient();
        }
};

#endif //API_H
