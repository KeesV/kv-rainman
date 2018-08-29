#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "settings.h"
#include "state.h"

class MqttClient {
    private:
    Settings settings;
    State state;
    WiFiClient espClient;
    PubSubClient mqttClient;

    unsigned long lastUpdated;

    public:
    MqttClient();
    void Start(Settings& settings, State& state);
    void Handle();
};

#endif