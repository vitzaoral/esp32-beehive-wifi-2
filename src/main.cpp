#include <Arduino.h>
#include <InternetConnection.h>
#include <Ticker.h>
#include "esp32-hal-cpu.h"

InternetConnection connection;
MeteoData meteoData;
PowerController powerController;
MagneticLockController magneticLockController;
MicrophoneController microphoneController;

void sendDataToInternet();
void checkMagneticLockAlarm();
void checkMicrophones();

Ticker timerSendDataToInternet(sendDataToInternet, 288000);  // 4.8 min 288000
Ticker timerMagneticLockAlarm(checkMagneticLockAlarm, 4321); // 4 sec
Ticker timerMicrophoneController(checkMicrophones, 281000);  // 4 TODO..

// alarm section
void sendDataToBlynkIfAlarm();
Ticker timerSendDataToBlynkIfAlarm(sendDataToBlynkIfAlarm, 20000); // 20 sec

void setup()
{
  Serial.begin(115200);

  // save battery, setup lower CPU frequency (default is 240Mhz)
  setCpuFrequencyMhz(80);
  Serial.println("Get CPU Frequency: " + String(getCpuFrequencyMhz()) + "Mhz");

  connection.initialize();

  timerSendDataToInternet.start();
  timerMagneticLockAlarm.start();
  timerSendDataToBlynkIfAlarm.start();

  meteoData.initializeSensors();

  // set first data for gyroscope and magnetic locks, other in timers..
  magneticLockController.setData();
  Serial.println("Setup done, send first data");
  sendDataToInternet();
  Serial.println("First data sended, start loop");
}

void loop()
{
  timerSendDataToInternet.update();
  timerMagneticLockAlarm.update();
  timerSendDataToBlynkIfAlarm.update();
  timerMicrophoneController.update();

  connection.blynkRunIfAlarm();
}

void sendDataToInternet()
{
  Serial.println("Start initialize Blynk connection");
  if (connection.initializeConnection())
  {
    Serial.println("Setting sensors data");
    meteoData.setData();
    powerController.setData();
    // gyroscope and magnetic locks data are set in other timer more often, so we have actual data

    Serial.println("Sending data to Blynk");
    connection.sendDataToBlynk(meteoData, powerController, magneticLockController, microphoneController);
    connection.checkNewVersionAndUpdate();
    connection.disconnect();
  }
  else
  {
    Serial.println("No internet/blynk connection");
    connection.disconnect();
  }
}

void checkMagneticLockAlarm()
{
  magneticLockController.setData();
  if (magneticLockController.isOk())
  {
    // update blynk data and turn alarm off
    if (connection.isAlarm)
    {
      connection.setMagneticLockControllerDataToBlynkIfAlarm(magneticLockController);
    }
    connection.isAlarm = false;
  }
  else
  {
    connection.isAlarm = true;
    connection.alarmMagneticController(magneticLockController);
  }
}

void sendDataToBlynkIfAlarm()
{
  connection.setMagneticLockControllerDataToBlynkIfAlarm(magneticLockController);
}

void checkMicrophones()
{
  microphoneController.setData();
}
