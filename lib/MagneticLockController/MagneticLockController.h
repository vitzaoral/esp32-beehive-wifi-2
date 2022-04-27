#ifndef __MagneticLockController_H
#define __MagneticLockController_H

#include <Arduino.h>

// bottoms
#define LOCK_D_PIN 25
#define LOCK_E_PIN 26
#define LOCK_F_PIN 27

// roofs
// #define LOCK_ROOF_D_PIN 14
// #define LOCK_ROOF_E_PIN 18
// #define LOCK_ROOF_F_PIN 13

struct LockData
{
  bool locked;
  String status;
};

class MagneticLockController
{
public:
  MagneticLockController();
  
  // bottoms
  LockData sensorD;
  LockData sensorE;
  LockData sensorF;

  // roofs
  // LockData sensorRoofD;
  // LockData sensorRoofE;
  // LockData sensorRoofF;

  void setData();
  bool isOk();
  String getAlarmMessage();

private:
  void setSensorData(LockData *data, int);
};

#endif