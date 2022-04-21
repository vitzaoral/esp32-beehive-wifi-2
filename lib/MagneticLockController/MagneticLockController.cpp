#include "MagneticLockController.h"

MagneticLockController::MagneticLockController()
{
}

void MagneticLockController::setData()
{
    // clear "wires memory"
    // on long cables it works slowly, so set tu LOW and check if is HIGH
    pinMode(LOCK_D_PIN, OUTPUT);
    pinMode(LOCK_E_PIN, OUTPUT);
    pinMode(LOCK_F_PIN, OUTPUT);
    pinMode(LOCK_ROOF_D_PIN, OUTPUT);
    pinMode(LOCK_ROOF_E_PIN, OUTPUT);
    pinMode(LOCK_ROOF_F_PIN, OUTPUT);

    digitalWrite(LOCK_D_PIN, LOW);
    digitalWrite(LOCK_E_PIN, LOW);
    digitalWrite(LOCK_F_PIN, LOW);
    digitalWrite(LOCK_ROOF_D_PIN, LOW);
    digitalWrite(LOCK_ROOF_E_PIN, LOW);
    digitalWrite(LOCK_ROOF_F_PIN, LOW);

    delay(200);
    pinMode(LOCK_D_PIN, INPUT);
    pinMode(LOCK_E_PIN, INPUT);
    pinMode(LOCK_F_PIN, INPUT);
    pinMode(LOCK_ROOF_D_PIN, INPUT);
    pinMode(LOCK_ROOF_E_PIN, INPUT);
    pinMode(LOCK_ROOF_F_PIN, INPUT);

    // bottoms
    Serial.print("Magnetic lock D: ");
    setSensorData(&sensorD, LOCK_D_PIN);
    Serial.print("Magnetic lock E: ");
    setSensorData(&sensorE, LOCK_E_PIN);
    Serial.print("Magnetic lock F: ");
    setSensorData(&sensorF, LOCK_F_PIN);

    // Roofs
    Serial.print("Magnetic roof lock D: ");
    setSensorData(&sensorRoofD, LOCK_ROOF_D_PIN);
    Serial.print("Magnetic roof lock E: ");
    setSensorData(&sensorRoofE, LOCK_ROOF_E_PIN);
    Serial.print("Magnetic roof lock F: ");
    setSensorData(&sensorRoofF, LOCK_ROOF_F_PIN);
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
    return sensorD.locked &&
           sensorE.locked &&
           sensorF.locked &&
           //sensorRoofD.locked && // TODO: not working
           sensorRoofE.locked &&
           sensorRoofF.locked;
}

// alarm message for alarm notification
String MagneticLockController::getAlarmMessage()
{
    String message = "";
    return String(sensorD.locked ? "" : "D") +
           String(sensorE.locked ? "" : "E") +
           String(sensorF.locked ? "" : "F") +
           String(sensorRoofD.locked ? "" : "r-D") +
           String(sensorRoofE.locked ? "" : "r-E") +
           String(sensorRoofF.locked ? "" : "r-F");
}