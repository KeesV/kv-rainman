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
}

void Screen::UpdateEntireScreen() {
    this->DisplayStationStatus(1, this->wateringStations[0]->IsWatering());
    this->DisplayStationStatus(2, this->wateringStations[1]->IsWatering());
    this->DisplayStationStatus(3, this->wateringStations[2]->IsWatering());
    this->DisplayStationStatus(4, this->wateringStations[3]->IsWatering());
    this->DisplayStationStatus(5, this->wateringStations[4]->IsWatering());
    this->DisplayStationStatus(6, this->wateringStations[5]->IsWatering());
    
    this->display.display();
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