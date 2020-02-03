// Project settings
#include "IPAddress.h"

struct Settings
{
    const char *wifiSSID = "Zaoralci";
    const char *wifiPassword = "74901Vita1";
    const char *blynkAuth = "G5MaJRaDEftlB_SNriq7XK2f1Td056AJ";
    const char *version = "1.0.0";
    const char *firmwareVersionUrl = "http://example.com/version.txt";
    const char *firmwareBinUrl = "http://example.com/firmware.bin";
    IPAddress ip = IPAddress(192, 168, 43, 000); // TODO: 000
    IPAddress gateway = IPAddress(192, 168, 43, 1);
    IPAddress subnet = IPAddress(255, 255, 255, 0);
};