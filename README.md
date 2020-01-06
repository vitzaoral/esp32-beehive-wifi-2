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
* triggers an alarm (Blynk notification) when someone opens the hive - a magnetic contact sensor
* triggers an alarm (Blynk notification) when someone picks up the hive - a magnetic contact sensor

### Currents list:

* [ESP32 WROOM-32](https://www.aliexpress.com/item/ESP32-ESP-32-ESP32S-ESP-32S-CP2102-Wireless-WiFi-Bluetooth-Development-Board-Micro-USB-Dual-Core/32867696371.html)
* [SHT30 digital temperature/humidity sensor](https://www.aliexpress.com/item/32962846003.html)
* [INA219 sensor](https://www.aliexpress.com/item/32846796535.html)
* [Magnetic contact sensor](https://www.aliexpress.com/item/32840447389.html)
* Solar panel 12V with battery and solar charger or other power supply

### Save battery power:
* Set CPU frequency to 30% (80Mhz) - *setCpuFrequencyMhz(80);*
* Setup lowest WiFi power - *WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);*
* Setup static IP, Gateway and DNS (quicker WiFi connection) - *WiFi.config(settings.ip, settings.gateway, settings.subnet, settings.gateway);*
* When disconnect WiFi, turn off WiFi modem - *WiFi.disconnect(true);*

### Schema:
![Schema](https://github.com/vitzaoral/esp32-beehive-wifi-2/blob/master/schema/schema.png)

### Powering 5V -> 3.3V:
3v3 regulator - https://randomnerdtutorials.com/esp8266-voltage-regulator-lipo-and-li-ion-batteries/