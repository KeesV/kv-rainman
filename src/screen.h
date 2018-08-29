#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "wateringStation.h"

#define OLED_RESET -1

class Screen {
    public:
    Screen();
    void Start(std::vector<WateringStation*> stations);
    void Handle();

    private:
    std::vector<WateringStation*> wateringStations;
    Adafruit_SSD1306 display;
    void DisplayStationStatus(int stationNumber, bool status);
    void UpdateEntireScreen();
};