#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <pgmspace.h>

#include "wateringStationManager.h"
#include "weather_icons.h"
#include "status_icons.h"
#include "rainmanStatus.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1

class Screen
{
public:
    Screen();
    void Start(RainmanStatus *status, WateringStationManager *stationManager);
    void Handle();

private:
    RainmanStatus *status;
    WateringStationManager *stationManager;
    Adafruit_SSD1306 display;
    void DisplayStationStatus(int stationNumber, bool status);
    void DisplayWeather();
    void DisplayConnectionStatus();
    void UpdateEntireScreen();

    unsigned long lastupdated;
    int currentImage;

    std::vector<const uint8_t *> weather_icons;
    std::vector<const uint8_t *> status_icons;
};
