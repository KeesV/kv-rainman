#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {

}

void MqttClient::Start(Settings& settings, std::vector<WateringStation*> stations) {
    this->settings = settings;
    this->wateringStations = stations;
    this->lastUpdated = millis();
}

void MqttClient::Handle() {
    if(millis() - this->lastUpdated > 2000) {
        //Serial.println("Toggling state for station #2...");
        for(int i = 0; i <=5; i++)
        {
            if(this->wateringStations[i]->IsWatering()) {
                this->wateringStations[i]->StopWatering();
            } else {
                this->wateringStations[i]->StartWatering();
            }
        }
        this->lastUpdated = millis();
    }
}