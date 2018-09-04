#include "screen.h"

Screen::Screen() : display(OLED_RESET) {
}

void Screen::Start(std::vector<WateringStation*> stations) {
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    // init done
    this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    this->display.clearDisplay();

    this->wateringStations = stations;

    this->UpdateEntireScreen();

    this->lastupdated = millis();
    this->currentImage = icon_cloudy;
}

void Screen::UpdateEntireScreen() {
    //this->DisplayWeather();
    for(int i = 0; i<=5; i++)
    {
        this->DisplayStationStatus(this->wateringStations[i]->GetNumber(), this->wateringStations[i]->IsWatering());
    }
   
    this->display.display();
}

void Screen::DisplayWeather()
{
    if(this->lastupdated - millis() > 1000) {
        // Display next image
        this->lastupdated = millis();
    }
    this->display.drawBitmap(38, 1, this->currentImage, 48, 48, WHITE);
}

void Screen::DisplayStationStatus(int stationNumber, bool status)
{
    int frontColor = status ? BLACK : WHITE;
    int backColor = status ? WHITE : BLACK;
    int n = stationNumber - 1;
    this->display.fillRoundRect(n*21 + 1, 43, 20, 20, 4, backColor);
    if(!status) {
        this->display.drawRoundRect(n*21 + 1, 43, 20, 20, 4, frontColor);
    }
    this->display.drawChar(n*21 + 6, 46, stationNumber + '0', frontColor, backColor, 2);
}

void Screen::Handle() {
    this->UpdateEntireScreen();
}