#include "MagneticLockController.h"

MagneticLockController::MagneticLockController()
{
    // bottoms
    pinMode(LOCK_A_PIN, INPUT);
    pinMode(LOCK_B_PIN, INPUT);
    pinMode(LOCK_C_PIN, INPUT);

    // roofs
    pinMode(LOCK_ROOF_A_PIN, INPUT);
    pinMode(LOCK_ROOF_B_PIN, INPUT);
    pinMode(LOCK_ROOF_C_PIN, INPUT);
}

void MagneticLockController::setData()
{
    // bottoms
    Serial.print("Magnetic lock A: ");
    setSensorData(&sensorA, LOCK_A_PIN);
    Serial.print("Magnetic lock B: ");
    setSensorData(&sensorB, LOCK_B_PIN);
    Serial.print("Magnetic lock C: ");
    setSensorData(&sensorC, LOCK_C_PIN);

    // Roofs
    Serial.print("Magnetic roof lock A: ");
    setSensorData(&sensorA, LOCK_ROOF_A_PIN);
    Serial.print("Magnetic roof lock B: ");
    setSensorData(&sensorB, LOCK_ROOF_B_PIN);
    Serial.print("Magnetic roof lock C: ");
    setSensorData(&sensorC, LOCK_ROOF_C_PIN);
}

void MagneticLockController::setSensorData(LockData *data, int pin)
{
    if (digitalRead(pin) == HIGH)
    {
        data->locked = true;
        data->status = "OK";
        Serial.println("OK");
    }
    else
    {
        data->locked = false;
        data->status = "ALARM!";
        Serial.println("UNLOCKED!");
    }
}

// Check if all lockers are locked
bool MagneticLockController::isOk()
{
    return sensorA.locked &&
           sensorB.locked &&
           sensorC.locked &&
           sensorRoofA.locked &&
           sensorRoofB.locked &&
           sensorRoofC.locked;
}

// alarm message for alarm notification
String MagneticLockController::getAlarmMessage()
{
    String message = "";
    return String(sensorA.locked ? "" : "A") +
           String(sensorB.locked ? "" : "B") +
           String(sensorC.locked ? "" : "C") +
           String(sensorRoofA.locked ? "" : "rA") +
           String(sensorRoofB.locked ? "" : "rB") +
           String(sensorRoofC.locked ? "" : "rC");
}