#include "MeteoData.h"

// SHT3X sensors
Adafruit_SHT31 sht31 = Adafruit_SHT31();

MeteoData::MeteoData()
{
    // one of the sensors is read on the address 0x44, others are set to the address 0x45 etc.
    pinMode(SENSOR_A_PIN, OUTPUT);
    pinMode(SENSOR_B_PIN, OUTPUT);
    pinMode(SENSOR_C_PIN, OUTPUT);
    digitalWrite(SENSOR_A_PIN, LOW);  // 0x44
    digitalWrite(SENSOR_B_PIN, HIGH); // 0x45
    digitalWrite(SENSOR_C_PIN, HIGH); // 0x45
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
    int delayTime = 10;

    digitalWrite(SENSOR_A_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor A :");
    MeteoData::setSensorData(&sensorA);
    digitalWrite(SENSOR_A_PIN, HIGH); // 0x45

    digitalWrite(SENSOR_B_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor B :");
    MeteoData::setSensorData(&sensorB);
    digitalWrite(SENSOR_B_PIN, HIGH); // 0x45

    digitalWrite(SENSOR_C_PIN, LOW); // 0x44
    delay(delayTime);
    Serial.print("Sensor C :");
    MeteoData::setSensorData(&sensorC);
    digitalWrite(SENSOR_C_PIN, HIGH); // 0x45
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