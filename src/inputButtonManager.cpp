#include "inputButtonManager.h"

InputButtonManager::InputButtonManager()
{
}

void InputButtonManager::Setup(WateringStationManager *stationManager)
{
    this->stationManager = stationManager;
    pinMode(this->LATCH, OUTPUT);
    pinMode(this->CLOCK, OUTPUT);
    pinMode(this->DATA, INPUT);
    this->Switch = B00000000;
    this->PreviousSwitch = B00000000;
    Serial.println("Initialized input button manager");
}

void InputButtonManager::Handle()
{
    digitalWrite(CLOCK, HIGH);
    digitalWrite(LATCH, LOW);
    digitalWrite(LATCH, HIGH);

    Switch = shiftIn(DATA, CLOCK, MSBFIRST);
    if (Switch != PreviousSwitch)
    {
        Serial.print("Read button state: ");
        for (byte i = 0; i < 8; i++)
        {
            Serial.print((Switch & (1 << i)) != 0 ? 0 : 1);
        }
        Serial.println();

        for (byte i = 0; i < 8; i++)
        {
            bool prevState = ((PreviousSwitch & (1 << i)) != 0 ? false : true);
            bool currenState = ((Switch & (1 << i)) != 0 ? false : true);
            if (!prevState && currenState)
            {
                this->stationManager->ToggleWatering(i);
            }
        }

        PreviousSwitch = Switch;
    }
}

bool InputButtonManager::GetButtonStatus(int buttonNumber)
{
    return (Switch & (1 << buttonNumber)) != 0;
}
