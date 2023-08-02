# IKEA_Tradfri_Lamp_Controller
 a REST-API on a ESP-12E Board for controlling the IKEA-Tradfri-Lamp with a "Hacked" Remote


Must add in /lib folder the file `WifiSecrets.hpp` with following content:

```C++
#ifndef WIFISECRETS_H
#define WIFISECRETS_H

#include <Arduino.h>

struct WifiSecrets {
    static constexpr char* WifiSSID = "<SSID>";
    static constexpr char* WifiPassword = "<Password>";
};

#endif
```