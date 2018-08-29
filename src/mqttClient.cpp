#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {

}

void MqttClient::Start(Settings& settings, State& state) {
    this->settings = settings;
    this->state = state;

    this->lastUpdated = millis();
}

void MqttClient::Handle() {
    if(millis() - this->lastUpdated > 2000) {
        Serial.println("Toggling state for station #2...");
        this->state.SetStationStatus(2, !this->state.GetStationStatus(2));
        this->lastUpdated = millis();
    }
}