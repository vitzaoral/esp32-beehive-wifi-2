#ifndef __PowerController_H
#define __PowerController_H

#include <Adafruit_INA219.h>

struct PowerData
{
  float busVoltage;
  float shuntVoltage;
  float loadVoltage;
  float current_mA;
  float power_mW;
};

class PowerController
{
public:
  PowerController();
  PowerData sensor_solar;
  PowerData sensor_battery;
  void setData();

private:
  void setData(Adafruit_INA219, PowerData *data);
};

#endif