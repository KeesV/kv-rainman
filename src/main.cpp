#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"
#include "screen.h"
#include "settings.h"
#include "wateringStationManager.h"
#include "mqttClient.h"
#include "rainmanStatus.h"

const char *ssid = "";
const char *password = "";

Settings settings;
Screen screen;
MqttClient mqttClient;
RainmanStatus status;
WateringStationManager stationManager;

void setup()
{
    Serial.begin(115200);
    settings.Begin();

    stationManager.Setup();

    screen.Start(&status, &stationManager);

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
    mqttClient.Start(&settings, &status, &stationManager);
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