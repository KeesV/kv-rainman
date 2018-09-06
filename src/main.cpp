#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"
#include "screen.h"
#include "settings.h"
#include "wateringStation.h"
#include "mqttClient.h"
#include "rainmanStatus.h"

const char *ssid = "";
const char *password = "";

Settings settings;
Screen screen;
MqttClient mqttClient;
RainmanStatus *status = new RainmanStatus();

std::vector<WateringStation *> wateringStations{
    new WateringStation(),
    new WateringStation(),
    new WateringStation(),
    new WateringStation(),
    new WateringStation(),
    new WateringStation()};

void setup()
{
    Serial.begin(115200);

    wateringStations[0]->Setup(1, D0);
    wateringStations[1]->Setup(2, D1);
    wateringStations[2]->Setup(3, D2);
    wateringStations[3]->Setup(4, D3);
    wateringStations[4]->Setup(5, D4);
    wateringStations[5]->Setup(6, D5);

    screen.Start(status, wateringStations);

    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    would try to act as both a client and an access-point and could cause
    network-issues with your other WiFi-devices on your WiFi-network. */

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    settings.DumpToSerial();

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    start_webserver(settings);
    mqttClient.Start(settings, status, wateringStations);
}

void handle_status()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        status->SetWifiStatus(t_RainmanConnectionStatus::ConnectionNOK);
    }
    else
    {
        status->SetWifiStatus(t_RainmanConnectionStatus::ConnectionOK);
    }
}

void loop()
{
    handle_status();
    yield();
    handle_webserver();
    yield();
    mqttClient.Handle();
    yield;
    screen.Handle();
    yield();
}