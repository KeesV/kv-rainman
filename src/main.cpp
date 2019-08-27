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
RainmanStatus status;

WateringStation station1;
WateringStation station2;
WateringStation station3;
WateringStation station4;
WateringStation station5;
WateringStation station6;

std::vector<WateringStation *> wateringStations{
    &station1,
    &station2,
    &station3,
    &station4,
    &station5,
    &station6};

void setup()
{
    Serial.begin(115200);
    settings.Begin();

    station1.Setup(1, D0);
    station2.Setup(2, D1);
    station3.Setup(3, D2);
    station4.Setup(4, D3);
    station5.Setup(5, D4);
    station6.Setup(6, D5);

    screen.Start(&status, wateringStations);

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

    start_webserver(&settings);
    mqttClient.Start(&settings, &status, wateringStations);
}

void handle_status()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        status.SetWifiStatus(t_RainmanConnectionStatus::ConnectionNOK);
    }
    else
    {
        status.SetWifiStatus(t_RainmanConnectionStatus::ConnectionOK);
    }
}

void loop()
{
    handle_status();
    yield();
    handle_webserver();
    yield();
    mqttClient.Handle();
    yield();
    screen.Handle();
    yield();
}