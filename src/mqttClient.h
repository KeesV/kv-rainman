#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "settings.h"
#include "wateringStationManager.h"
#include "rainmanStatus.h"

class MqttClient
{
private:
    Settings *settings;
    RainmanStatus *status;
    WiFiClient espClient;
    PubSubClient mqttClient;
    WateringStationManager *stationManager;
    std::vector<bool> previousWateringStationStates;
    void messageReceivedCallback(char *p_topic, byte *p_payload, unsigned int p_length);
    void mqttReconnect();

public:
    MqttClient();
    void Start(Settings *settings, RainmanStatus *status, WateringStationManager *stationManager);
    void Handle();
};

#endif