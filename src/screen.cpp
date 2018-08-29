#include "screen.h"

Screen::Screen() : display(OLED_RESET) {
}

void Screen::Start(WateringStation* station1) {
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    // init done
    this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    this->display.clearDisplay();

    this->station1 = station1;

    this->UpdateEntireScreen();
}

void Screen::UpdateEntireScreen() {
    this->DisplayStationStatus(1, this->station1->IsWatering());
    this->DisplayStationStatus(2, false);
    this->DisplayStationStatus(3, true);
    this->DisplayStationStatus(4, false);
    this->DisplayStationStatus(5, true);
    this->DisplayStationStatus(6, false);
    
    this->display.display();
}

void Screen::DisplayStationStatus(int stationNumber, bool status)
{
    int frontColor = status ? BLACK : WHITE;
    int backColor = status ? WHITE : BLACK;
    int n = stationNumber - 1;
    if(status) {
        this->display.fillRoundRect(n*21 + 1, 43, 20, 20, 4, backColor);
    } else {
        this->display.drawRoundRect(n*21 + 1, 43, 20, 20, 4, frontColor);
    }
    this->display.drawChar(n*21 + 6, 46, stationNumber + '0', frontColor, backColor, 2);
}

void Screen::Handle() {
    this->UpdateEntireScreen();
}