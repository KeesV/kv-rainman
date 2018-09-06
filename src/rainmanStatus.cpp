#include "rainmanStatus.h"

RainmanStatus::RainmanStatus() {
    this->mqttStatus = t_RainmanConnectionStatus::ConnectionNOK;
    this->wifiStatus = t_RainmanConnectionStatus::ConnectionNOK;
    this->currentWeather = t_RainmanWeather::UNKNOWN;
}

t_RainmanConnectionStatus RainmanStatus::GetWifiStatus() {
    return this->wifiStatus;
}

t_RainmanConnectionStatus RainmanStatus::GetMqttStatus() {
    return this->mqttStatus;
}

t_RainmanWeather RainmanStatus::GetWeather() {
    return this->currentWeather;
}

void RainmanStatus::SetWifiStatus(t_RainmanConnectionStatus status)
{
    this->wifiStatus = status;
}

void RainmanStatus::SetMqttStatus(t_RainmanConnectionStatus status)
{
    this->mqttStatus = status;
}

void RainmanStatus::SetWeather(t_RainmanWeather weather)
{
    this->currentWeather = weather;
}

