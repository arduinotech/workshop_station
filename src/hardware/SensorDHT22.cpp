#include <SPI.h>
#include "SensorDHT22.h"

SensorDHT22::SensorDHT22(uint8_t pin)
{
    _pin = pin;
}

void SensorDHT22::init()
{
    pinMode(_pin, INPUT);
    _dht = new DHT(_pin, DHT22);
    _dht->begin();
}

int8_t SensorDHT22::getTemp()
{
    return static_cast<int8_t>(_dht->readTemperature());
}

int8_t SensorDHT22::getHumi()
{
    return static_cast<int8_t>(_dht->readHumidity());
}