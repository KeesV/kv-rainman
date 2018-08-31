#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {
}

void MqttClient::Start(Settings& settings, std::vector<WateringStation*> stations) {
    this->settings = settings;
    this->wateringStations = stations;
    this->lastUpdated = millis();
    const char* ip = "192.168.1.63";

    for(int i = 0; i<=5; i++) {
        this->previousWateringStationStates.push_back(this->wateringStations[i]->IsWatering());
    }

    this->mqttClient.setServer(ip, 1883);
    this->mqttClient.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->messageReceivedCallback(topic, payload, length); });
}

void MqttClient::Handle() {
    if (!this->mqttClient.connected()) {
        this->mqttReconnect();
    }

    // Find if any of the station states changed
    for(int i = 0; i<=5; i++) {
        if(this->previousWateringStationStates[i] != this->wateringStations[i]->IsWatering()) {
            Serial.print("Station ");
            Serial.print(i+1);
            Serial.println(" changed state!");
            String mqttStateTopicBase = this->settings.GetMqttStateTopicBase();
            String mqttStateTopic = mqttStateTopicBase + "/" + (i+1);
            String payload = this->wateringStations[i]->IsWatering() 
                ? this->settings.GetMqttPayloadOn()
                : this->settings.GetMqttPayloadOff();
            Serial.print("Publishing [");
            Serial.print(mqttStateTopic);
            Serial.print("]: ");
            Serial.println(payload);
            this->mqttClient.publish(mqttStateTopic.c_str(), payload.c_str());
            this->previousWateringStationStates[i] = this->wateringStations[i]->IsWatering();
        }
    }

    this->mqttClient.loop();
}

void MqttClient::messageReceivedCallback(char* p_topic, byte* p_payload, unsigned int p_length) {
    Serial.print("Message arrived [");
    Serial.print(p_topic);
    Serial.print("] ");
    
    // concat the payload into a string
    String payload;
    for (uint8_t i = 0; i < p_length; i++) {
        payload.concat((char)p_payload[i]);
    }
    Serial.println(payload);

    String topic(p_topic);
    String mqttCommandTopicBase = this->settings.GetMqttCommandTopicBase();
    if(topic.startsWith(mqttCommandTopicBase)) {
        Serial.print("Received on command topic for station: ");
        String station = topic.substring(mqttCommandTopicBase.length()+1);
        Serial.println(station);
        int stationNr = station.toInt();
        if(stationNr > 0 && stationNr <= 6)
        {
            if(payload == this->settings.GetMqttPayloadOn()) {
                this->wateringStations[stationNr-1]->StartWatering();
            } else if(payload == this->settings.GetMqttPayloadOff()) {
                this->wateringStations[stationNr-1]->StopWatering();
            } else {
                Serial.println("No valid payload!");
            }
        } else {
            Serial.println("No valid station!");
        }
    }
}

void MqttClient::mqttReconnect() {
    // Loop until we're reconnected
    while (!this->mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "rainman-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (this->mqttClient.connect(clientId.c_str())) {
            Serial.println("MQTT connected!");
            
            String mqttCommandTopicBase = this->settings.GetMqttCommandTopicBase();
            String mqttCommandSubscribtion = mqttCommandTopicBase + "/+";

            Serial.print("Subscribing to topic: ");
            Serial.println(mqttCommandSubscribtion.c_str());
            mqttClient.subscribe(mqttCommandSubscribtion.c_str());
        } else {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(this->mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}