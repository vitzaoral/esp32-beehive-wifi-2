# ESP32 beehive (WiFi version)
Beehives online monitoring based on the ESP32 and others sensors. WiFi version.

> To build a project, you need to download all the necessary libraries and create the *settings.cpp* file in the *src* folder:
```c++
// Project settings
#include "IPAddress.h"

struct Settings
{
    const char *wifiSSID = "YYY";
    const char *wifiPassword = "ZZZ";
    const char *version = "1.0.0";
    const char *firmwareVersionUrl = "http://example.com/version.txt";
    const char *firmwareBinUrl = "http://example.com/firmware.bin";
    IPAddress ip = IPAddress(192, 168, 43, 223);
    IPAddress gateway = IPAddress(192, 168, 43, 1);
    IPAddress subnet = IPAddress(255, 255, 255, 0);
};
```
### Features
* measuring the temperature and humidity inside the hive
* measuring the sound level inside the hive
* triggers an alarm (Blynk notification) when someone opens the hive - a magnetic contact sensor
* triggers an alarm (Blynk notification) when someone picks up the hive - a magnetic contact sensor

### Currents list:

* [ESP32-WROOM-32U](https://www.aliexpress.com/item/4000103411061.html)
* 3x [SHT30 digital temperature/humidity sensor](https://www.aliexpress.com/item/32962846003.html)
* 2x [INA219 sensor](https://www.aliexpress.com/item/32846796535.html)
* 3x [GY-MAX4466 microphone](https://www.aliexpress.com/item/32974988349.html)
* 3x [Magnetic contact sensor](https://www.aliexpress.com/item/32840447389.html)
* [12V MPPT Solar Panel Controller for 18650 Battery](https://www.aliexpress.com/item/32947501783.html)
* 1x - 4x [3.7v 3400 mAh 18650 Lithium battery](https://www.aliexpress.com/item/32807032859.html)
* Solar panel 12V 12W (or 2x [6V 6W panels](https://www.aliexpress.com/item/32901764415.html) in [series](http://sunbeam-bd.blogspot.com/p/blog-page_26.html))
* 3x 9 pins COM cabels and some male/female connectors

### Save battery power:
* Set CPU frequency to 30% (80Mhz) - *setCpuFrequencyMhz(80);*
* Setup lowest WiFi power - *WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);*
* Setup static IP, Gateway and DNS (quicker WiFi connection) - *WiFi.config(settings.ip, settings.gateway, settings.subnet, settings.gateway);*
* When disconnect WiFi, turn off WiFi modem - *WiFi.disconnect(true);*

### Schema:
![Schema](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/schema.png)

![Circuit](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/IMG_9549.jpg)

![Circuit](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/IMG_9550.jpg)

![Circuit](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/IMG_9551.jpg)

![Circuit](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/IMG_9555.jpg)

![Circuit](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/IMG_9556.jpg)

### Powering 5V -> 3.3V:
3v3 regulator - https://randomnerdtutorials.com/esp8266-voltage-regulator-lipo-and-li-ion-batteries/