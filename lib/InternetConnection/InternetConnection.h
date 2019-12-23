#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <Update.h>
#include <HTTPUpdate.h>
#include <HTTPClient.h>
#include <MeteoData.h>
#include <PowerController.h>
#include <MagneticLockController.h>

class InternetConnection
{
public:
  void initialize();
  bool isAlarm;
  bool initializeConnection();
  void disconnect();
  void sendDataToBlynk(MeteoData, PowerController, MagneticLockController);
  void alarmMagneticController(MagneticLockController);
  void blynkRunIfAlarm();
  void setMagneticLockControllerDataToBlynkIfAlarm(MagneticLockController);
  void checkNewVersionAndUpdate();
  void printlnToTerminal(String);

private:
  void setMagneticLockControllerDataToBlynk(MagneticLockController);
  void setAlarmInfoToBlynk();
  void setAlarmCollor(int, bool);
  void updateFirmware();
  void setI2CStatusVersion();
};

#endif