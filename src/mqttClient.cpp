#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {

}

void MqttClient::Start(Settings& settings, WateringStation* station1) {
    this->settings = settings;
    this->station1 = station1;
    this->lastUpdated = millis();
}

void MqttClient::Handle() {
    if(millis() - this->lastUpdated > 2000) {
        //Serial.println("Toggling state for station #2...");
        if(this->station1->IsWatering()) {
            this->station1->StopWatering();
        } else {
            this->station1->StartWatering();
        }
        this->lastUpdated = millis();
    }
}