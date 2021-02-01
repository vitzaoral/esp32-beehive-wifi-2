#include "MeteoData.h"

// SHT3X sensors
Adafruit_SHT31 sht31 = Adafruit_SHT31();

MeteoData::MeteoData()
{
    // one of the sensors is read on the address 0x44, others are set to the address 0x45 etc.
    pinMode(SENSOR_D_PIN, OUTPUT);
    pinMode(SENSOR_E_PIN, OUTPUT);
    pinMode(SENSOR_F_PIN, OUTPUT);

    digitalWrite(SENSOR_D_PIN, LOW);  // 0x44
    digitalWrite(SENSOR_E_PIN, HIGH); // 0x45
    digitalWrite(SENSOR_F_PIN, HIGH); // 0x45
}

void MeteoData::initializeSensors()
{
    if (!sht31.begin(0x44))
    {
        Serial.println("Could not find a valid SHT31X sensor on oaddress 0x44!");
    }
    else
    {
        Serial.println("Inner SHT31X OK");
    }
}

void MeteoData::setData()
{
    int delayTime = 20;
    delay(delayTime);

    // D
    digitalWrite(SENSOR_D_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor D :");
    MeteoData::setSensorData(&sensorD);
    digitalWrite(SENSOR_D_PIN, HIGH); // 0x45

    // E
    digitalWrite(SENSOR_E_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor E :");
    MeteoData::setSensorData(&sensorE);
    digitalWrite(SENSOR_E_PIN, HIGH); // 0x45

    // F
    digitalWrite(SENSOR_F_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor F :");
    MeteoData::setSensorData(&sensorF);
    digitalWrite(SENSOR_F_PIN, HIGH); // 0x45
}

void MeteoData::setSensorData(TempAndHumidity *data)
{
    data->temperature = sht31.readTemperature();
    data->humidity = sht31.readHumidity();
    MeteoData::printSensorData(data);
}

void MeteoData::printSensorData(TempAndHumidity *data)
{
    Serial.print("temperature: " + String(data->temperature) + "°C ");
    Serial.print("humidity: " + String(data->humidity) + "% ");
}