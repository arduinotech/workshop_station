#ifndef SensorDHT22_h
#define SensorDHT22_h

#include <Arduino.h>
#include <DHT.h>

class SensorDHT22
{
    public:
        SensorDHT22(uint8_t pin);
        void init();
        int8_t getTemp();
        int8_t getHumi();
    private:
        uint8_t _pin;
        DHT* _dht;
};

#endif