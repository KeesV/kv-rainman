#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {
}

void MqttClient::Start(Settings& settings, RainmanStatus* status, std::vector<WateringStation*> stations) {
    this->settings = settings;
    this->status = status;
    this->wateringStations = stations;
    this->mqttHost = this->settings.GetMqttBrokerHost();
    this->mqttPort = this->settings.GetMqttBrokerPort().toInt();
    this->mqttShouldRetain = this->settings.GetMqttRetain();

    for(int i = 0; i<=5; i++) {
        this->previousWateringStationStates.push_back(this->wateringStations[i]->IsWatering());
    }

    Serial.print("Connecting to MQTT broker ");
    Serial.print(this->mqttHost.c_str());
    Serial.print(", port ");
    Serial.println(this->mqttPort);
    this->mqttClient.setServer(this->mqttHost.c_str(), this->mqttPort);
    this->mqttClient.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->messageReceivedCallback(topic, payload, length); });
}

void MqttClient::Handle() {
    if (!this->mqttClient.connected()) {
        this->status->SetMqttStatus(t_RainmanConnectionStatus::ConnectionNOK);
        this->mqttReconnect();
        return;
    }
    this->status->SetMqttStatus(t_RainmanConnectionStatus::ConnectionOK);

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
            this->mqttClient.publish(mqttStateTopic.c_str(), payload.c_str(), this->mqttShouldRetain);
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
    String mqttWeatherTopic = this->settings.GetMqttWeatherTopic();

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
    } else if (topic.startsWith(mqttWeatherTopic)) {
        Serial.println("Received on weather topic.");
        t_RainmanWeather w=t_RainmanWeather::UNKNOWN;
        if(payload == "cloudy") { w = t_RainmanWeather::CLOUDY; }
        if(payload == "fog") { w = t_RainmanWeather::FOG; }
        if(payload == "hail") { w = t_RainmanWeather::HAIL; }
        if(payload == "lightning") { w = t_RainmanWeather::LIGHTNING; }
        if(payload == "lightning-rainy") { w = t_RainmanWeather::LIGHTNING_RAINY; }
        if(payload == "partlycloudy") { w = t_RainmanWeather::PARTLYCLOUDY; }
        if(payload == "pouring") { w = t_RainmanWeather::POURING; }
        if(payload == "rainy") { w = t_RainmanWeather::RAINY; }
        if(payload == "snowy") { w = t_RainmanWeather::SNOWY; }
        if(payload == "snowy-rainy") { w = t_RainmanWeather::SNOWY_RAINY; }
        if(payload == "sunny") { w = t_RainmanWeather::SUNNY; }
        if(payload == "windy") { w = t_RainmanWeather::WINDY; }
        if(payload == "windy-variant") { w = t_RainmanWeather::WINDY_VARIANT; }
        this->status->SetWeather(w);
    }
}

void MqttClient::mqttReconnect() {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "rainman-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (this->mqttClient.connect(clientId.c_str())) {
        Serial.println("MQTT connected!");
        
        String mqttCommandTopicBase = this->settings.GetMqttCommandTopicBase();
        String mqttCommandSubscribtion = mqttCommandTopicBase + "/+";

        if(mqttCommandTopicBase.length() > 0) {
            Serial.print("Subscribing to topic: ");
            Serial.println(mqttCommandSubscribtion.c_str());
            mqttClient.subscribe(mqttCommandSubscribtion.c_str());
        } else {
            Serial.println("MqttCommandTopicBase not set, cannot subscribe!");
        }

        String mqttWeatherTopic = this->settings.GetMqttWeatherTopic();
        
        if(mqttWeatherTopic.length() > 0) {
            Serial.print("Subscribing to topic: ");
            Serial.println(mqttWeatherTopic);
            mqttClient.subscribe(mqttWeatherTopic.c_str());
        } else {
            Serial.println("mqttWeatherTopic not set, cannot subscribe!");
        }
    } else {
        Serial.print("MQTT connect failed, rc=");
        Serial.println(this->mqttClient.state());
    }
}