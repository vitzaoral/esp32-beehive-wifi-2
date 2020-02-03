#ifndef __MagneticLockController_H
#define __MagneticLockController_H

#include <Arduino.h>

// bottoms
#define LOCK_A_PIN 25
#define LOCK_B_PIN 26
#define LOCK_C_PIN 27

// roofs
#define LOCK_ROOF_A_PIN 35
#define LOCK_ROOF_B_PIN 32
#define LOCK_ROOF_C_PIN 33

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
  LockData sensorA;
  LockData sensorB;
  LockData sensorC;

  // roofs
  LockData sensorRoofA;
  LockData sensorRoofB;
  LockData sensorRoofC;

  void setData();
  bool isOk();
  String getAlarmMessage();

private:
  void setSensorData(LockData *data, int);
};

#endif