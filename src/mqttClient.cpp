#include "mqttClient.h"

bool startsWith(const char *str, const char *pre)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

MqttClient::MqttClient() : mqttClient(espClient)
{
}

void MqttClient::Start(Settings *settings, RainmanStatus *status, WateringStationManager *stationManager)
{
    this->settings = settings;
    this->status = status;
    this->stationManager = stationManager;

    for (int i = 0; i <= 5; i++)
    {
        this->previousWateringStationStates.push_back(this->stationManager->IsWatering(i));
    }

    Serial.print("Connecting to MQTT broker ");
    Serial.print(this->settings->GetMqttBrokerHost());
    Serial.print(", port ");
    Serial.println(this->settings->GetMqttBrokerPort());
    this->mqttClient.setServer(this->settings->GetMqttBrokerHost(), this->settings->GetMqttBrokerPort());
    this->mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length) { this->messageReceivedCallback(topic, payload, length); });
}

void MqttClient::Handle()
{
    if (!this->mqttClient.connected())
    {
        this->status->SetMqttStatus(t_RainmanConnectionStatus::ConnectionNOK);
        this->mqttReconnect();
        return;
    }
    this->status->SetMqttStatus(t_RainmanConnectionStatus::ConnectionOK);

    // Find if any of the station states changed
    for (int i = 0; i <= 5; i++)
    {
        if (this->previousWateringStationStates[i] != this->stationManager->IsWatering(i))
        {
            Serial.print("Station ");
            Serial.print(i + 1);
            Serial.println(" changed state!");

            // Construct topic to publish to
            char mqttStateTopic[MqttStateTopicBaseLength + 3];
            strcpy(mqttStateTopic, this->settings->GetMqttStateTopicBase());
            strcat(mqttStateTopic, "/");
            char stationNumberStr[2];
            sprintf(stationNumberStr, "%d", i + 1);
            strcat(mqttStateTopic, stationNumberStr);

            char *payload = this->stationManager->IsWatering(i)
                                ? this->settings->GetMqttPayloadOn()
                                : this->settings->GetMqttPayloadOff();
            Serial.print("Publishing [");
            Serial.print(mqttStateTopic);
            Serial.print("]: ");
            Serial.println(payload);
            this->mqttClient.publish(mqttStateTopic, payload, this->settings->GetMqttRetain());
            this->previousWateringStationStates[i] = this->stationManager->IsWatering(i);
        }
    }

    this->mqttClient.loop();
}

void MqttClient::messageReceivedCallback(char *p_topic, byte *p_payload, unsigned int p_length)
{
    Serial.print("Message arrived [");
    Serial.print(p_topic);
    Serial.print("]: ");

    // concat the payload into a string
    char payload[p_length + 1];
    for (uint8_t i = 0; i < p_length; i++)
    {
        payload[i] = (char)p_payload[i];
    }
    payload[p_length] = '\0';
    Serial.print("'");
    Serial.print(payload);
    Serial.println("'");

    if (startsWith(p_topic, this->settings->GetMqttCommandTopicBase()))
    {
        Serial.print("Received on command topic for station: ");
        // Take everything from te received topic string, after the base topic string
        char station[3];
        memcpy(
            station,
            &p_topic[strlen(this->settings->GetMqttCommandTopicBase()) + 1],
            strlen(p_topic) - strlen(this->settings->GetMqttCommandTopicBase()));
        Serial.println(station);
        int stationNr;
        sscanf(station, "%d", &stationNr);
        if (stationNr > 0 && stationNr <= 6)
        {
            if (strcmp(payload, this->settings->GetMqttPayloadOn()) == 0)
            {
                this->stationManager->StartWatering(stationNr - 1);
            }
            else if (strcmp(payload, this->settings->GetMqttPayloadOff()) == 0)
            {
                this->stationManager->StopWatering(stationNr - 1);
            }
            else
            {
                Serial.println("No valid payload!");
            }
        }
        else
        {
            Serial.println("No valid station!");
        }
    }
    else if (startsWith(p_topic, this->settings->GetMqttWeatherTopic()))
    {
        Serial.println("Received on weather topic.");
        t_RainmanWeather w = t_RainmanWeather::UNKNOWN;
        if (strcmp(payload, "cloudy") == 0)
        {
            w = t_RainmanWeather::CLOUDY;
        }
        if (strcmp(payload, "fog") == 0)
        {
            w = t_RainmanWeather::FOG;
        }
        if (strcmp(payload, "hail") == 0)
        {
            w = t_RainmanWeather::HAIL;
        }
        if (strcmp(payload, "lightning") == 0)
        {
            w = t_RainmanWeather::LIGHTNING;
        }
        if (strcmp(payload, "lightning-rainy") == 0)
        {
            w = t_RainmanWeather::LIGHTNING_RAINY;
        }
        if (strcmp(payload, "partlycloudy") == 0)
        {
            w = t_RainmanWeather::PARTLYCLOUDY;
        }
        if (strcmp(payload, "pouring") == 0)
        {
            w = t_RainmanWeather::POURING;
        }
        if (strcmp(payload, "rainy") == 0)
        {
            w = t_RainmanWeather::RAINY;
        }
        if (strcmp(payload, "snowy") == 0)
        {
            w = t_RainmanWeather::SNOWY;
        }
        if (strcmp(payload, "snowy-rainy") == 0)
        {
            w = t_RainmanWeather::SNOWY_RAINY;
        }
        if (strcmp(payload, "sunny") == 0)
        {
            w = t_RainmanWeather::SUNNY;
        }
        if (strcmp(payload, "windy") == 0)
        {
            w = t_RainmanWeather::WINDY;
        }
        if (strcmp(payload, "windy-variant") == 0)
        {
            w = t_RainmanWeather::WINDY_VARIANT;
        }
        this->status->SetWeather(w);
    }
}

void MqttClient::mqttReconnect()
{
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    char str[8 + 1] = {0};
    int i;
    for (i = 0; i < 8; i += 2)
    {
        sprintf(&str[i], "%02X", rand() % 16);
    }

    char clientId[17];
    strcpy(clientId, "rainman-");
    strcat(clientId, str);
    Serial.print("Client id: ");
    Serial.println(clientId);

    // Attempt to connect
    if (this->mqttClient.connect(clientId))
    {
        Serial.println("MQTT connected!");

        char *mqttCommandTopicBase = this->settings->GetMqttCommandTopicBase();
        char mqttCommandSubscribtion[MqttCommandTopicBaseLength + 2];
        strcpy(mqttCommandSubscribtion, mqttCommandTopicBase);
        strcat(mqttCommandSubscribtion, "/+");

        if (strlen(mqttCommandTopicBase) > 0)
        {
            Serial.print("Subscribing to topic: ");
            Serial.println(mqttCommandSubscribtion);
            mqttClient.subscribe(mqttCommandSubscribtion);
        }
        else
        {
            Serial.println("MqttCommandTopicBase not set, cannot subscribe!");
        }

        char *mqttWeatherTopic = this->settings->GetMqttWeatherTopic();

        if (strlen(mqttWeatherTopic) > 0)
        {
            Serial.print("Subscribing to topic: ");
            Serial.println(mqttWeatherTopic);
            mqttClient.subscribe(mqttWeatherTopic);
        }
        else
        {
            Serial.println("mqttWeatherTopic not set, cannot subscribe!");
        }
    }
    else
    {
        Serial.print("MQTT connect failed, rc=");
        Serial.println(this->mqttClient.state());
    }
}