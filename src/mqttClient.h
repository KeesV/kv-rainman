#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "settings.h"
#include "wateringStation.h"

class MqttClient {
    private:
    Settings settings;
    WiFiClient espClient;
    PubSubClient mqttClient;
    std::vector<WateringStation*> wateringStations;
    std::vector<bool> previousWateringStationStates;
    void messageReceivedCallback(char* p_topic, byte* p_payload, unsigned int p_length);
    void mqttReconnect();

    unsigned long lastUpdated;

    public:
    MqttClient();
    void Start(Settings& settings, std::vector<WateringStation*> stations);
    void Handle();
};

#endif