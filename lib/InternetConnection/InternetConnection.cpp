#include "InternetConnection.h"
#include <BlynkSimpleEsp32.h>
#include "../../src/settings.cpp"

// hard reset
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

Settings settings;

// HTTP Clients for OTA over WiFi
WiFiClient client;

// Attach Blynk virtual serial terminal
WidgetTerminal terminal(V36);

// OTA - firmware file name for OTA update on the SPIFFS
const char firmwareFile[] = "/firmware.bin";
// OTA - Number of milliseconds to wait without receiving any data before we give up
const int otaNetworkTimeout = 30 * 1000;
// OTA - Number of milliseconds to wait if no data is available before trying again
const int otakNetworkDelay = 1000;

// alarm notifications are enabled
bool alarmEnabledNotifications = false;

// alarm is enabled
bool alarmIsEnabled = false;

// start OTA update process
bool startOTA = false;

#define SDA 21
#define SCL 22

// ESP32 hard reset  via mosfet: connect GND to EN pin => reset
#define RESET_PIN 2

// need lot of blynk virtual pins :)
#define BLYNK_USE_128_VPINS

// Synchronize settings from Blynk server with device when internet is connected
BLYNK_CONNECTED()
{
    Blynk.syncAll();
}

// Restart ESP
BLYNK_WRITE(V0)
{
    if (param.asInt())
    {
        Blynk.virtualWrite(V0, false);
        Serial.println("Restarting, bye..");
        terminal.write("Restarting, bye..");
        terminal.flush();
        delay(500);
        // hard reset
        digitalWrite(RESET_PIN, HIGH);
    }
}

// Turn on/off alarm notifications
BLYNK_WRITE(V30)
{
    alarmEnabledNotifications = param.asInt();
    Serial.println("Alarm notifications was " + String(alarmEnabledNotifications ? "enabled" : "disabled"));
}

// Turn on/off alarm
BLYNK_WRITE(V31)
{
    alarmIsEnabled = param.asInt();
    Serial.println("Alarm was " + String(alarmIsEnabled ? "enabled" : "disabled"));
}

// Terminal input
BLYNK_WRITE(V36)
{
    String valueFromTerminal = param.asStr();

    if (String("clear") == valueFromTerminal)
    {
        terminal.clear();
        terminal.println("CLEARED");
        terminal.flush();
    }
    else if (String("update") == valueFromTerminal)
    {
        terminal.clear();
        terminal.println("Start OTA enabled");
        terminal.flush();
        startOTA = true;
    }
    else if (valueFromTerminal != "\n" || valueFromTerminal != "\r" || valueFromTerminal != "")
    {
        terminal.println(String("unknown command: ") + valueFromTerminal);
        terminal.flush();
    }
}

InternetConnection::InternetConnection()
{
    // true if is actual alarm - run Blynk.run
    isAlarm = false;
    // for reset via mosfet -> en pin
    pinMode(RESET_PIN, OUTPUT);
}

bool InternetConnection::initializeConnection()
{
    int connAttempts = 0;
    Serial.println("\r\nTry connecting to: " + String(settings.wifiSSID));

    //save battery power, set lowest WiFi power
    WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
    delay(1);

    // try config - quicker for WiFi connection
    // TODO:
    //WiFi.config(settings.ip, settings.gateway, settings.subnet, settings.gateway);

    WiFi.begin(settings.wifiSSID, settings.wifiPassword);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (connAttempts > 20)
        {
            Serial.println("Error - couldn't connect to WIFI");
            return false;
        }

        connAttempts++;
    }

    delay(1000);

    Serial.println("\r\nConnecting to WIFI OK, connnecting to Blynk");

    if (!Blynk.connected())
    {
        Blynk.config(settings.blynkAuth);
        // timeout 6sec
        Blynk.connect(2000);
    }

    return Blynk.connected();
}

void InternetConnection::disconnect()
{
    if (WiFi.isConnected())
    {
        Blynk.disconnect();
        WiFi.disconnect(true);

        Serial.println("Disconnected OK");
    }
    else
    {
        Serial.println("Already disconnected");
    }
}

void InternetConnection::sendDataToBlynk(
    MeteoData meteoData,
    PowerController powerController,
    MagneticLockController magneticLockController,
    MicrophoneController microphoneController)
{
    // create data to send to Blynk
    if (Blynk.connected())
    {
        // meteo data A
        Blynk.virtualWrite(V4, meteoData.sensorA.humidity);
        Blynk.virtualWrite(V5, meteoData.sensorA.temperature);

        // meteo data B
        Blynk.virtualWrite(V6, meteoData.sensorB.humidity);
        Blynk.virtualWrite(V7, meteoData.sensorB.temperature);

        // solar power data
        Blynk.virtualWrite(V8, powerController.sensor_solar.loadVoltage);
        Blynk.virtualWrite(V9, powerController.sensor_solar.current_mA);
        Blynk.virtualWrite(V10, powerController.sensor_solar.power_mW);

        // battery power data
        Blynk.virtualWrite(V41, powerController.sensor_battery.loadVoltage);
        Blynk.virtualWrite(V42, powerController.sensor_battery.current_mA);
        Blynk.virtualWrite(V43, powerController.sensor_battery.power_mW);

        // 3100mV is minimum value, 4204mV is maximum value, difference is 1104mV
        int percent = ((powerController.sensor_battery.loadVoltage - 3.1) / 1.104) * 100.0;
        Blynk.virtualWrite(V2, percent);

        // meteo data C
        Blynk.virtualWrite(V12, meteoData.sensorC.humidity);
        Blynk.virtualWrite(V13, meteoData.sensorC.temperature);

        // microphones
        Blynk.virtualWrite(V44, microphoneController.sensorD.leq);
        Blynk.virtualWrite(V45, microphoneController.sensorE.leq);
        Blynk.virtualWrite(V46, microphoneController.sensorF.leq);

        bool isMicrophonesNan = isnan(microphoneController.sensorD.leq) &&
                                isnan(microphoneController.sensorE.leq) &&
                                isnan(microphoneController.sensorF.leq);
        if (isMicrophonesNan)
        {
            Blynk.notify("Microphones is NaN");
        }

        // set SDA/SCL status
        setI2CStatusVersion();

        // magnetic locks data
        setMagneticLockControllerDataToBlynk(magneticLockController);

        // set alarm info
        setAlarmInfoToBlynk();

        // WIFI info
        Blynk.virtualWrite(V39, "IP: " + WiFi.localIP().toString() + "|G: " + WiFi.gatewayIP().toString() + "|S: " + WiFi.subnetMask().toString() + "|DNS: " + WiFi.dnsIP().toString());
        Blynk.virtualWrite(V40, WiFi.RSSI());

        Blynk.run();
        delay(1000);

        Serial.println("Sending data to Blynk - DONE");
    }
    else
    {
        Serial.println("Blynk is not connected");
    }
}

void InternetConnection::setI2CStatusVersion()
{
    // I2C status - SDA and SCL
    bool SDAisOK = digitalRead(SDA);
    bool SCLsOK = digitalRead(SCL);

    String message = (SDAisOK ? String("SDA OK, ") : String("SDA error, ")) +
                     (SCLsOK ? String("SCL OK, ") : String("SCL error, ")) +
                     String("Version: ") + settings.version;

    Blynk.virtualWrite(V17, message);
}

void InternetConnection::setMagneticLockControllerDataToBlynk(MagneticLockController magneticLockController)
{
    Blynk.virtualWrite(V19, magneticLockController.sensorA.status);
    Blynk.virtualWrite(V20, magneticLockController.sensorB.status);
    Blynk.virtualWrite(V21, magneticLockController.sensorC.status);

    Blynk.virtualWrite(V14, magneticLockController.sensorRoofA.status);
    Blynk.virtualWrite(V15, magneticLockController.sensorRoofB.status);
    Blynk.virtualWrite(V16, magneticLockController.sensorRoofC.status);

    setAlarmInfoToBlynk();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ALARM SECTION
////////////////////////////////////////////////////////////////////////////////////////////////////

void InternetConnection::setAlarmInfoToBlynk()
{
    Blynk.virtualWrite(V32, isAlarm ? "AKTUÁLNÍ ALARM!" : "OK");
    Blynk.virtualWrite(V33, alarmEnabledNotifications ? "Alarm notifikace zapnuty" : "Alarm notifikace vypnuty");
    Blynk.virtualWrite(V34, alarmIsEnabled ? "Alarm zapnut" : "Alarm vypnut");

    if (isAlarm)
    {
        Serial.println("\n !! ALARM !! \n");
    }
}

void InternetConnection::blynkRunIfAlarm()
{
    if (alarmIsEnabled && isAlarm)
    {
        Blynk.run();
    }
}

void InternetConnection::setMagneticLockControllerDataToBlynkIfAlarm(MagneticLockController magneticLockController)
{
    if (isAlarm)
    {
        setMagneticLockControllerDataToBlynk(magneticLockController);
    }
}

void InternetConnection::alarmMagneticController(MagneticLockController magneticLockController)
{
    if (!alarmIsEnabled)
    {
        return;
    }

    Serial.println("\n!!! Magnetic alarm !!!\n");

    if (!Blynk.connected())
    {
        initializeConnection();
    }

    if (Blynk.connected())
    {
        setMagneticLockControllerDataToBlynk(magneticLockController);

        if (alarmEnabledNotifications)
        {
            Blynk.notify("! ALARM ! Magnetický zámek je otevřen: " + magneticLockController.getAlarmMessage());
        }
    }
    else
    {
        Serial.println("ALARM but can't connected to Blynk");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// OTA SECTION
////////////////////////////////////////////////////////////////////////////////////////////////////

void InternetConnection::checkNewVersionAndUpdate()
{
    Serial.println("Checking new firmware version..");
    if (!startOTA)
    {
        Serial.println("OTA - not setted");
        return;
    }
    else
    {
        startOTA = false;
    }

    printlnToTerminal("Start OTA, check internet connection");

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi is not connected!");
        return;
    }

    HTTPClient http;
    http.begin(client, settings.firmwareVersionUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        String version = http.getString();
        Serial.println("Version: " + version);

        if (String(settings.version) != version)
        {
            printlnToTerminal("!!! START OTA UPDATE !!!");
            updateFirmware();
        }
        else
        {
            printlnToTerminal("Already on the latest version");
        }
    }
    else
    {
        printlnToTerminal("Failed verify version from server, status code: " + String(httpCode));
    }

    http.end();
    Serial.println("Restart after OTA, bye");
    delay(1000);

    // hard reset
    esp_task_wdt_init(1, true);
    esp_task_wdt_add(NULL);
    while (true)
        ;
    //ESP.restart();
}

void InternetConnection::updateFirmware()
{
    t_httpUpdate_return ret = httpUpdate.update(client, settings.firmwareBinUrl); /// FOR ESP32 HTTP OTA
    printlnToTerminal("OTA RESULT: " + String(ret));

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); /// FOR ESP32
        // char currentString[64];
        // sprintf(currentString, "\nHTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); /// FOR ESP32
        // Serial.println(currentString);
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("OTA OK");
        break;
    }
    ESP.restart();
}

/// Println message to serial and Blynk terminal
void InternetConnection::printlnToTerminal(String message)
{
    Serial.println(message);
    terminal.println(message);
    terminal.flush();
}