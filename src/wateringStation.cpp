#include "wateringStation.h"

WateringStation::WateringStation() {
}

void WateringStation::Setup(int number, int pin) {
    this->watering = false;
    this->pin = pin;
    this->number = number;

    pinMode(pin, OUTPUT);

    Serial.print("Initialized watering station ");
    Serial.println(this->number);
}

bool WateringStation::IsWatering() {
    return this->watering;
}

void WateringStation::StartWatering() {
    this->watering = true;
    Serial.print("Station ");
    Serial.print(this->number);
    Serial.println(": started watering");
}

void WateringStation::StopWatering() {
    this->watering = false;
    Serial.print("Station ");
    Serial.print(this->number);
    Serial.println(": stopped watering");
}