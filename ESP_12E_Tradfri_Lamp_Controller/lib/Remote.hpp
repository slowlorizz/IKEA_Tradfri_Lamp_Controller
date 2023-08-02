#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>

class Remote
{
    private:
        constexpr static int power_gpio = 12; // D6
        constexpr static int brighter_gpio = 14; // D5
        constexpr static int darker_gpio = 4; // D2
        constexpr static int left_gpio = 5; // D1
        constexpr static int right_gpio = 2; // D4

        constexpr static int PHASE_TIME = 500;
        constexpr static int BRIGHTNESS_PHASE_TIME = 1000;
        constexpr static int COLOR_PHASE_TIME = 1100;

        inline static void press_btn(int btn_gpio){
            digitalWrite(btn_gpio, LOW);
            delay((btn_gpio == Remote::left_gpio || btn_gpio == Remote::right_gpio ? Remote::COLOR_PHASE_TIME : (btn_gpio == Remote::brighter_gpio || btn_gpio == Remote::darker_gpio ? Remote::BRIGHTNESS_PHASE_TIME : PHASE_TIME)));
            digitalWrite(btn_gpio, HIGH);
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
        }

        inline static void brighter(){
            Remote::press_btn(Remote::brighter_gpio);
        }

        inline static void darker(){
            Remote::press_btn(Remote::darker_gpio);
        }

        inline static void color_left(){
            Remote::press_btn(Remote::left_gpio);
        }

        inline static void color_right(){
            Remote::press_btn(Remote::right_gpio);
        }
};

#endif //REMOTE_H
