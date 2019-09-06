#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"
#include "screen.h"
#include "settings.h"
#include "state.h"
#include "mqttClient.h"

const char *ssid = "";
const char *password = "";

State state;
Settings settings;
Screen screen;
MqttClient mqttClient;

void setup()
{
    Serial.begin(115200);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);

    screen.Start(state);

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
    mqttClient.Start(settings, state);
}

void loop()
{
    handle_webserver();
    yield();
    mqttClient.Handle();
    yield;
    screen.Handle();
    yield();
}