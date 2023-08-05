#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>

namespace LampStats
{
    bool power = true;
    int lumen = 5;
    int color = 0;

    const int lumen_range[2] = {0, 5};
    const int color_range[2] = {0, 8};

    namespace Event 
    {
        void power_toggled() {
            LampStats::power = !LampStats::power;

            Serial.print("power: "); Serial.println((LampStats::power ? "On" : "Off"));
        }

        void brighter() {
            if(LampStats::lumen < LampStats::lumen_range[1]) {
                LampStats::lumen += 1;
            }

            Serial.print("lumen: "); Serial.println(LampStats::lumen);
        }

        void darker() {
            if(LampStats::lumen > LampStats::lumen_range[0]){
                LampStats::lumen -= 1;
            }

            Serial.print("lumen: "); Serial.println(LampStats::lumen);
        }

        void left() {
            if (LampStats::color < LampStats::color_range[1]){
                LampStats::color += 1;
            }
            else {
                LampStats::color = LampStats::color_range[0];
            }

            Serial.print("color: "); Serial.println(LampStats::color);
        }

        void right() {
            if (LampStats::color > LampStats::color_range[0]){
                LampStats::color -= 1;
            }
            else {
                LampStats::color = LampStats::color_range[1];
            }

            Serial.print("color: "); Serial.println(LampStats::color);
        }
    }
}

class Remote
{
    private:
        constexpr static int power_gpio = 12; // D6
        constexpr static int brighter_gpio = 14; // D5
        constexpr static int darker_gpio = 4; // D2
        constexpr static int left_gpio = 5; // D1
        constexpr static int right_gpio = 9; // SD2

        constexpr static int PHASE_TIME = 500;
        constexpr static int LUMEN_PHASE_TIME = 1000;
        constexpr static int COLOR_PHASE_TIME = 1100;

        inline static void press_btn(int btn_gpio){
            digitalWrite(btn_gpio, LOW);
            delay((btn_gpio == Remote::left_gpio || btn_gpio == Remote::right_gpio ? Remote::COLOR_PHASE_TIME : (btn_gpio == Remote::brighter_gpio || btn_gpio == Remote::darker_gpio ? Remote::LUMEN_PHASE_TIME : PHASE_TIME)));
            digitalWrite(btn_gpio, HIGH);
        }

        inline static void set_default_lumen(){
            for(int i = 0; i < LampStats::lumen_range[1]; ++i){
                Remote::brighter();
                delay(0);
            }
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
            LampStats::Event::power_toggled();
        }

        inline static void brighter(){
            Remote::press_btn(Remote::brighter_gpio);
            LampStats::Event::brighter();
        }

        inline static void darker(){
            Remote::press_btn(Remote::darker_gpio);
            LampStats::Event::darker();
        }

        inline static void color_left(){
            Remote::press_btn(Remote::left_gpio);
            LampStats::Event::left();
        }

        inline static void color_right(){
            Remote::press_btn(Remote::right_gpio);
            LampStats::Event::right();
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

            Remote::set_default_lumen();
        }

        inline static void set_power(bool state){
            if(LampStats::power != state){
                Remote::toggle_power();
            }
        }

        inline static void set_lumen(int lvl){
            if (LampStats::lumen < lvl){
                while (LampStats::lumen < lvl && LampStats::lumen != LampStats::lumen_range[1]){
                    Remote::brighter();
                    delay(0);
                }
            }
            else if(LampStats::lumen > lvl){
                while (LampStats::lumen > lvl && LampStats::lumen != LampStats::lumen_range[0]){
                    Remote::darker();
                    delay(0);
                }
            }
        }

        inline static void set_color(int lvl){
            if(lvl > LampStats::color_range[1]){
                lvl = LampStats::color_range[1];
            }
            else if(lvl < LampStats::color_range[0])
            {
                lvl = LampStats::color_range[0];
            }
            

            if (LampStats::color < lvl){
                while(LampStats::color < lvl){
                    Remote::color_left();
                    delay(1750);
                }
            }
            else if(LampStats::color > lvl){
                while (LampStats::color > lvl){
                    Remote::color_right();
                    delay(1750);
                }
            }
        }
};

#endif //REMOTE_H
