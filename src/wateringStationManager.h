#ifndef __WATERINGSTATIONMANAGER_H__
#define __WATERINGSTATIONMANAGER_H__

#include <Arduino.h>

#define DATAPIN 12  //Outputs the byte to transfer
#define LOADPIN 13  //Controls the internal transference of data in SN74HC595 internal registers
#define CLOCKPIN 15 //Generates the clock signal to control the transference of data

class WateringStationManager
{
private:
    byte wateringStationStatus;
    void SetData();

public:
    WateringStationManager();
    void Setup();
    bool IsWatering(int stationNumber);
    void StartWatering(int stationNumber);
    void StopWatering(int stationNumber);
};

#endif