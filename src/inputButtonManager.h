#ifndef __INPUTBUTTONMANAGER_H__
#define __INPUTBUTTONMANAGER_H__

#include <Arduino.h>
#include "wateringStationManager.h"

class InputButtonManager
{
private:
    const byte LATCH = D0;
    const byte DATA = D5;
    const byte CLOCK = D3;

    WateringStationManager *stationManager;
    byte Switch;
    byte PreviousSwitch;
    bool GetButtonStatus(int buttonNumber);

public:
    InputButtonManager();
    void Setup(WateringStationManager *stationManager);
    void Handle();
};

#endif