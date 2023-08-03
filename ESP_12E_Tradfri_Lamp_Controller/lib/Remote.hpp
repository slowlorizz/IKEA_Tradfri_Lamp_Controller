#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>

bool powerstate = true;
int brightness = 5;
int color = 0;

class Remote
{
    private:
        constexpr static int power_gpio = 12; // D6
        constexpr static int brighter_gpio = 14; // D5
        constexpr static int darker_gpio = 4; // D2
        constexpr static int left_gpio = 5; // D1
        constexpr static int right_gpio = 9; // SD2

        constexpr static int PHASE_TIME = 500;
        constexpr static int BRIGHTNESS_PHASE_TIME = 1000;
        constexpr static int COLOR_PHASE_TIME = 1100;

        inline static void press_btn(int btn_gpio){
            digitalWrite(btn_gpio, LOW);
            delay((btn_gpio == Remote::left_gpio || btn_gpio == Remote::right_gpio ? Remote::COLOR_PHASE_TIME : (btn_gpio == Remote::brighter_gpio || btn_gpio == Remote::darker_gpio ? Remote::BRIGHTNESS_PHASE_TIME : PHASE_TIME)));
            digitalWrite(btn_gpio, HIGH);
        }

        inline static void power_toggle_event(){
            powerstate = !powerstate;
        }

        inline static void brighter_event(){
            if(brightness < 5) {
                brightness += 1;
            }
        }

        inline static void darker_event(){
            if(brightness > 0){
                brightness -= 1;
            }
        }

        inline static void color_left_event(){
            if (color < 8){
                color += 1;
            }
            else {
                color = 0;
            }

            Serial.print("color: "); Serial.println(color);
        }

        inline static void color_right_event(){
            if(color > 0){
                color -= 1;
            }
            else {
                color = 8;
            }
            
            Serial.print("color: "); Serial.println(color);
        }

    public:
        inline static void register_pins(){
            pinMode(Remote::power_gpio, OUTPUT);
            pinMode(Remote::brighter_gpio, OUTPUT);
            pinMode(Remote::darker_gpio, OUTPUT);
            pinMode(Remote::left_gpio, OUTPUT);
            pinMode(Remote::right_gpio, OUTPUT);
        }

        inline static void toggle_power(){
            Remote::press_btn(Remote::power_gpio);
            Remote::power_toggle_event();
        }

        inline static void brighter(){
            Remote::press_btn(Remote::brighter_gpio);
            Remote::brighter_event();
        }

        inline static void darker(){
            Remote::press_btn(Remote::darker_gpio);
            Remote::darker_event();
        }

        inline static void color_left(){
            Remote::press_btn(Remote::left_gpio);
            Remote::color_left_event();
        }

        inline static void color_right(){
            Remote::press_btn(Remote::right_gpio);
            Remote::color_right_event();
        }

        inline static void wakeup_pins(){
            int act_timeout = 200;

            delay(act_timeout);
            Remote::press_btn(Remote::power_gpio);
            delay(act_timeout);
            Remote::press_btn(Remote::darker_gpio);
            delay(act_timeout);
            Remote::press_btn(Remote::brighter_gpio);
            delay(act_timeout);
            Remote::press_btn(Remote::right_gpio);
            delay(act_timeout);
            Remote::press_btn(Remote::left_gpio);
            delay(act_timeout);

            for(int i = 0; i < 5; i++){
                Remote::brighter();
                delay(200);
            }
        }

        inline static void set_power(bool state){
            if(powerstate != state){
                Remote::toggle_power();
            }
        }

        inline static void set_brightness(int lvl){
            if (brightness < lvl){
                while (brightness < lvl && brightness != 5){
                    Remote::brighter();
                    delay(0);
                }
            }
            else if(brightness > lvl){
                while (brightness > lvl && brightness != 0){
                    Remote::darker();
                    delay(0);
                }
            }
        }

        inline static void set_color(int lvl){
            if(lvl > 8){
                lvl = 8;
            }
            else if(lvl < 0)
            {
                lvl = 0;
            }
            

            if (color < lvl){
                while(color < lvl){
                    Remote::color_left();
                    delay(1750);
                }
            }
            else if(color > lvl){
                while (color > lvl){
                    Remote::color_right();
                    delay(1750);
                }
            }
        }
};

#endif //REMOTE_H
