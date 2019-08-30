#include "wateringStationManager.h"

void printByte(byte val)
{
    for (int i = 0; i < 8; i++)
    {
        bool b = val & 0x80;
        Serial.print(b);
        val = val << 1;
    }
}

WateringStationManager::WateringStationManager()
{
}

void WateringStationManager::Setup()
{
    this->wateringStationStatus = B11111111;
    pinMode(DATAPIN, OUTPUT);
    pinMode(LOADPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    this->SetData();
    Serial.println("Initialized watering station manager");
}

void WateringStationManager::SetData()
{
    Serial.print("Going to write data: ");
    printByte(this->wateringStationStatus);
    Serial.println();

    digitalWrite(LOADPIN, LOW);
    shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, this->wateringStationStatus);
    digitalWrite(LOADPIN, HIGH);
}

bool WateringStationManager::IsWatering(int stationNumber)
{
    bool status = ((B00000001 << stationNumber) & this->wateringStationStatus) == 0;
    return status;
}

void WateringStationManager::StartWatering(int stationNumber)
{
    // Set the station to be watered to 0, and all others to 1 (effectively only watering a single station)
    this->wateringStationStatus = ~(B00000001 << stationNumber);
    this->SetData();
}

void WateringStationManager::StopWatering(int stationNumber)
{
    // Simple implementation that will just stop watering all stations
    this->wateringStationStatus = B11111111;
    this->SetData();
}

void WateringStationManager::ToggleWatering(int stationNumber)
{
    if (IsWatering(stationNumber))
    {
        StopWatering(stationNumber);
    }
    else
    {
        StartWatering(stationNumber);
    }
}
