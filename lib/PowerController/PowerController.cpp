#include "PowerController.h"

// more ina219 sensors https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ina219-current-sensor-breakout.pdf stranka 6
Adafruit_INA219 ina219_solar;
Adafruit_INA219 ina219_battery(0x41);

PowerController::PowerController()
{
    ina219_solar.begin();
    ina219_solar.setCalibration_32V_2A();

    ina219_battery.begin();
    ina219_battery.setCalibration_32V_2A();
}

void PowerController::setData()
{
    Serial.println("Solar sensor:");
    setData(ina219_solar, &sensor_solar);

    Serial.println("12V battery sensor:");
    setData(ina219_battery, &sensor_battery);
}

void PowerController::setData(Adafruit_INA219 sensor, PowerData *data)
{
    data->shuntVoltage = sensor.getShuntVoltage_mV();
    data->busVoltage = sensor.getBusVoltage_V();
    data->current_mA = sensor.getCurrent_mA();
    data->power_mW = sensor.getPower_mW();
    data->loadVoltage = data->busVoltage + (data->shuntVoltage / 1000);

    Serial.print("Bus Voltage:   ");
    Serial.print(data->busVoltage);
    Serial.println(" V");
    Serial.print("Shunt Voltage: ");
    Serial.print(data->shuntVoltage);
    Serial.println(" mV");
    Serial.print("Load Voltage:  ");
    Serial.print(data->loadVoltage);
    Serial.println(" V");
    Serial.print("Current:       ");
    Serial.print(data->current_mA);
    Serial.println(" mA");
    Serial.print("Power:         ");
    Serial.print(data->power_mW);
    Serial.println(" mW");
}
