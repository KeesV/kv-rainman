#include "rainmanStatus.h"

RainmanStatus::RainmanStatus() {
    this->mqttStatus = t_RainmanConnectionStatus::ConnectionNOK;
    this->wifiStatus = t_RainmanConnectionStatus::ConnectionNOK;
}

t_RainmanConnectionStatus RainmanStatus::GetWifiStatus() {
    return this->wifiStatus;
}

t_RainmanConnectionStatus RainmanStatus::GetMqttStatus() {
    return this->mqttStatus;
}

void RainmanStatus::SetWifiStatus(t_RainmanConnectionStatus status)
{
    this->wifiStatus = status;
}

void RainmanStatus::SetMqttStatus(t_RainmanConnectionStatus status)
{
    this->mqttStatus = status;
}

