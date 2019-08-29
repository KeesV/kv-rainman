#include "screen.h"

Screen::Screen() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

void Screen::Start(RainmanStatus *status, WateringStationManager *stationManager)
{
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    // init done
    this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    this->display.clearDisplay();

    this->stationManager = stationManager;
    this->status = status;

    this->weather_icons.push_back(icon_cloudy);
    this->weather_icons.push_back(icon_fog);
    this->weather_icons.push_back(icon_hail);
    this->weather_icons.push_back(icon_lightning);
    this->weather_icons.push_back(icon_lightning_rainy);
    this->weather_icons.push_back(icon_partlycloudy);
    this->weather_icons.push_back(icon_pouring);
    this->weather_icons.push_back(icon_rainy);
    this->weather_icons.push_back(icon_snowy);
    this->weather_icons.push_back(icon_snowy_rainy);
    this->weather_icons.push_back(icon_sunny);
    this->weather_icons.push_back(icon_windy);
    this->weather_icons.push_back(icon_windy_variant);

    this->status_icons.push_back(icon_wifi_on);
    this->status_icons.push_back(icon_wifi_off);
    this->status_icons.push_back(icon_mqtt_connected);
    this->status_icons.push_back(icon_mqtt_disconnected);

    this->lastupdated = millis();
    this->currentImage = 0;

    this->UpdateEntireScreen();
}

void Screen::UpdateEntireScreen()
{
    this->DisplayWeather();
    this->DisplayConnectionStatus();

    for (int i = 0; i <= 5; i++)
    {
        this->DisplayStationStatus(i + 1, this->stationManager->IsWatering(i));
    }

    this->display.display();
}

void Screen::DisplayWeather()
{
    int weatherIcon = 0;

    switch (this->status->GetWeather())
    {
    case t_RainmanWeather::CLOUDY:
        weatherIcon = 0;
        break;
    case t_RainmanWeather::FOG:
        weatherIcon = 1;
        break;
    case t_RainmanWeather::HAIL:
        weatherIcon = 2;
        break;
    case t_RainmanWeather::LIGHTNING:
        weatherIcon = 3;
        break;
    case t_RainmanWeather::LIGHTNING_RAINY:
        weatherIcon = 4;
        break;
    case t_RainmanWeather::PARTLYCLOUDY:
        weatherIcon = 5;
        break;
    case t_RainmanWeather::POURING:
        weatherIcon = 6;
        break;
    case t_RainmanWeather::RAINY:
        weatherIcon = 7;
        break;
    case t_RainmanWeather::SNOWY:
        weatherIcon = 8;
        break;
    case t_RainmanWeather::SNOWY_RAINY:
        weatherIcon = 9;
        break;
    case t_RainmanWeather::SUNNY:
        weatherIcon = 10;
        break;
    case t_RainmanWeather::WINDY:
        weatherIcon = 11;
        break;
    case t_RainmanWeather::WINDY_VARIANT:
        weatherIcon = 12;
        break;
    case t_RainmanWeather::UNKNOWN:
        weatherIcon = 0;
        break;
    }

    this->display.fillRect(38, -3, 48, 48, BLACK);
    if (this->status->GetWeather() != t_RainmanWeather::UNKNOWN)
    {
        this->display.drawBitmap(38, -3, this->weather_icons[weatherIcon], 48, 48, WHITE);
    }
}

void Screen::DisplayConnectionStatus()
{
    this->display.fillRect(104, 0, 16, 16, BLACK);
    const uint8_t *wifiStatusImage = this->status->GetWifiStatus() == t_RainmanConnectionStatus::ConnectionOK
                                         ? this->status_icons[0]
                                         : this->status_icons[1];
    this->display.drawBitmap(104, 0, wifiStatusImage, 16, 16, WHITE);

    this->display.fillRect(104, 19, 16, 16, BLACK);
    const uint8_t *mqttStatusImage = this->status->GetMqttStatus() == t_RainmanConnectionStatus::ConnectionOK
                                         ? this->status_icons[2]
                                         : this->status_icons[3];
    this->display.drawBitmap(104, 19, mqttStatusImage, 16, 16, WHITE);
}

void Screen::DisplayStationStatus(int stationNumber, bool status)
{
    int frontColor = status ? BLACK : WHITE;
    int backColor = status ? WHITE : BLACK;
    int n = stationNumber - 1;
    this->display.fillRoundRect(n * 21 + 1, 43, 20, 20, 4, backColor);
    if (!status)
    {
        this->display.drawRoundRect(n * 21 + 1, 43, 20, 20, 4, frontColor);
    }
    this->display.drawChar(n * 21 + 6, 46, stationNumber + '0', frontColor, backColor, 2);
}

void Screen::Handle()
{
    this->UpdateEntireScreen();
}