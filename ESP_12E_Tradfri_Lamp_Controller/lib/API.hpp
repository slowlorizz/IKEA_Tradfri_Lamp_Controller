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
                API::post_success_answer();
                Remote::toggle_power();
            }
        }

        inline static void post_brighter_route(){
            if(WebServer.method() == HTTP_POST) {
                API::post_success_answer();
                Remote::brighter();
            }
        }

        inline static void post_darker_route(){
            if(WebServer.method() == HTTP_POST) {
                API::post_success_answer();
                Remote::darker();
            }
        }

        inline static void post_left_route(){
            if(WebServer.method() == HTTP_POST) {
                API::post_success_answer();
                Remote::color_left();
            }
        }

        inline static void post_right_route(){
            if(WebServer.method() == HTTP_POST) {
                API::post_success_answer();
                Remote::color_right();
            }
        }

        inline static void post_set_route(){
            if(WebServer.method() == HTTP_POST) {
                bool n_pwr_state = powerstate;
                int n_brt_lvl = brightness;
                int n_color_lvl = color;

                String n_brt_lvl_str = "";
                String n_color_lvl_str = "";

                if(WebServer.arg("power") != ""){
                    n_pwr_state = WebServer.arg("power") == "on" ? true : false;
                }

                if(WebServer.arg("brightness") != ""){
                    n_brt_lvl_str = (String)(WebServer.arg("brightness"));
                    n_brt_lvl = n_brt_lvl_str.toInt();
                }

                if(WebServer.arg("color") != ""){
                    n_color_lvl_str = (String)(WebServer.arg("color"));
                    n_color_lvl = n_color_lvl_str.toInt();
                }

                API::post_success_answer();

                Remote::set_power(n_pwr_state);
                Remote::set_brightness(n_brt_lvl);
                Remote::set_color(n_color_lvl);
            }
        }

        inline static void get_status_route(){
            if(WebServer.method() == HTTP_GET) {
                DynamicJsonDocument doc(1024);
                doc["status"] = "OK";
                doc["power"] = (powerstate ? "On" : "Off");
                doc["brightness"] = brightness;
                doc["color"] = color;
                String buf;
                serializeJson(doc, buf);
                WebServer.send(201, F("application/json"), buf);
            }
        }

        inline static void get_status_power_route(){
            if(WebServer.method() == HTTP_GET) {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["power"] = (powerstate ? "On" : "Off");
                String buf;
                serializeJson(doc, buf);
                WebServer.send(201, F("application/json"), buf);
            }
        }

        inline static void get_status_brightness_route(){
            if(WebServer.method() == HTTP_GET) {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["brightness"] = brightness;
                String buf;
                serializeJson(doc, buf);
                WebServer.send(201, F("application/json"), buf);
            }
        }

        inline static void get_status_color_route(){
            if(WebServer.method() == HTTP_GET) {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["color"] = color;
                String buf;
                serializeJson(doc, buf);
                WebServer.send(201, F("application/json"), buf);
            }
        }

    public:
        inline static void set_routes(){
            WebServer.on("/power", HTTP_POST, API::post_power_route);
            WebServer.on("/brighter", HTTP_POST, API::post_brighter_route);
            WebServer.on("/darker", HTTP_POST, API::post_darker_route);
            WebServer.on("/color/left", HTTP_POST, API::post_left_route);
            WebServer.on("/color/right", HTTP_POST, API::post_right_route);
            WebServer.on("/set", HTTP_POST, API::post_set_route);
            WebServer.on("/status", HTTP_GET, API::get_status_route);
            WebServer.on("/status/power", HTTP_GET, API::get_status_power_route);
            WebServer.on("/status/brightness", HTTP_GET, API::get_status_brightness_route);
            WebServer.on("/status/color", HTTP_GET, API::get_status_color_route);
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
