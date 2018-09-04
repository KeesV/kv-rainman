#ifndef __WATERINGSTATION_H__
#define __WATERINGSTATION_H__

#include <Arduino.h>

class WateringStation {
    private:
    bool watering;
    int number;
    int pin;

    public:
    WateringStation();
    void Setup(int number, int pin);
    bool IsWatering();
    void StartWatering();
    void StopWatering();
    int GetNumber();
};

#endif