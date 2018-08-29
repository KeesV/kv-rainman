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
    void Start(WateringStation* station1);
    void Handle();

    private:
    WateringStation* station1;
    Adafruit_SSD1306 display;
    void DisplayStationStatus(int stationNumber, bool status);
    void UpdateEntireScreen();
};