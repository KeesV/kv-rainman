#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "settings.h"
#include "wateringStation.h"
#include "rainmanStatus.h"

class MqttClient {
    private:
    Settings* settings;
    RainmanStatus* status;
    WiFiClient espClient;
    PubSubClient mqttClient;
    std::vector<WateringStation*> wateringStations;
    std::vector<bool> previousWateringStationStates;
    void messageReceivedCallback(char* p_topic, byte* p_payload, unsigned int p_length);
    void mqttReconnect();
    bool mqttShouldRetain;

    public:
    MqttClient();
    void Start(Settings* settings, RainmanStatus* status, std::vector<WateringStation*> stations);
    void Handle();
};

#endif