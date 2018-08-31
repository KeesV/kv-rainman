#include "mqttClient.h"

MqttClient::MqttClient() : mqttClient(espClient) {

}

void MqttClient::Start(Settings& settings, std::vector<WateringStation*> stations) {
    this->settings = settings;
    this->wateringStations = stations;
    this->lastUpdated = millis();
    const char* ip = "192.168.1.63";
    this->mqttClient.setServer(ip, 1883);
    this->mqttClient.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->messageReceivedCallback(topic, payload, length); });
}

void MqttClient::Handle() {
    if (!this->mqttClient.connected()) {
        this->mqttReconnect();
    }
    this->mqttClient.loop();

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

void MqttClient::messageReceivedCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // // Switch on the LED if an 1 was received as first character
    // if ((char)payload[0] == '1') {
    // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // // but actually the LED is on; this is because
    // // it is acive low on the ESP-01)
    // } else {
    // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    // }
}

void MqttClient::mqttReconnect() {
    const char* inTopic = "rainman/in";

    // Loop until we're reconnected
    while (!this->mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (this->mqttClient.connect(clientId.c_str())) {
            Serial.println("MQTT connected!");
            // Once connected, publish an announcement...
            //this->mqttClient.publish(outTopic, "hello world");
            // ... and resubscribe
            mqttClient.subscribe(inTopic);
        } else {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(this->mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}