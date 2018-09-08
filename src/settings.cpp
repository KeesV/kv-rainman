#include "settings.h"

Settings::Settings() { 
}

Settings::~Settings() {
}

void Settings::Begin() {
    EEPROM.begin(MAX_EEPROM_SIZE);
    this->ReadAllSettingsFromEeprom();
}

void saveToEeprom(String val, int startAddr, int maxLength) {
    // Write length of the string in the first 2 bytes of the field, so that we know how long to read when getting the value
    String length = String(val.length());
    if(length.length() < 2) { length = "0" + length; }
    Serial.print("Writing with length: ");
    Serial.println(length);
    EEPROM.write(startAddr, length[0]);
    EEPROM.write(startAddr + 1, length[1]);

    // Then write the actual data
    int addr = startAddr + 2;
    for (int i = 0; i < val.length() && i < maxLength-2; ++i)
    {
        EEPROM.write(addr, val[i]);
        Serial.print("Wrote: ");
        Serial.println(val[i]);
        addr++; 
    }
    EEPROM.commit();
}

void readFromEeprom(char* destValue, int startAddr, int maxLength)
{
    //char* value = (char*)malloc(sizeof(char)*(maxLength+1));

    //Read length from first 2 bytes of field
    char lengthString[3];
    lengthString[0] = char(EEPROM.read(startAddr));
    lengthString[1] = char(EEPROM.read(startAddr + 1));
    lengthString[2] = '\0';
    int lengthToRead;
    sscanf(lengthString, "%d", &lengthToRead);
    Serial.print("2- Going to read with length: ");
    Serial.println(lengthToRead);

    if(lengthToRead > 0)
    {
        int charNr = 0;
        for (int i = startAddr + 2; i < (2 + startAddr + lengthToRead) && (i - startAddr < maxLength - 2); ++i)
        {
            char c = char(EEPROM.read(i));
            destValue[charNr] = c;
            charNr ++;
        }
        destValue[charNr] = '\0';
        Serial.print("2- Read from EEPROM: ");
        Serial.println(destValue);
        
    } else {
        destValue[0] = '\0'; //If we don't have anything in EEPROM we'll just return an empty string
    }
}

void Settings::ReadAllSettingsFromEeprom() {
    readFromEeprom(this->MqttBrokerHost, MqttBrokerHostStartAddr, MqttBrokerHostLength);
    readFromEeprom(this->MqttCommandTopicBase, MqttCommandTopicBaseStartAddr, MqttCommandTopicBaseLength);
    readFromEeprom(this->MqttStateTopicBase, MqttStateTopicBaseStartAddr, MqttStateTopicBaseLength);
    readFromEeprom(this->MqttPayloadOn, MqttPayloadOnStartAddr, MqttPayloadOnLength);
    readFromEeprom(this->MqttPayloadOff, MqttPayloadOffStartAddr, MqttPayloadOffLength);
    readFromEeprom(this->MqttWeatherTopic, MqttWeatherTopicStartAddr, MqttWeatherTopicLength);

    char mqttRetainStr[MqttRetainLength+1];
    readFromEeprom(mqttRetainStr, MqttRetainStartAddr, MqttRetainLength);
    this->SetMqttRetain(mqttRetainStr);

    char mqttBrokerPortStr[MqttBrokerPortLength+1];
    readFromEeprom(mqttBrokerPortStr, MqttBrokerPortStartAddr, MqttBrokerPortLength);
    this->SetMqttBrokerPort(mqttBrokerPortStr);
}

void Settings::EraseAll() {
    for(int i = 0; i < MAX_EEPROM_SIZE; i++ ) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    this->ReadAllSettingsFromEeprom();
}

void Settings::Save() {
    saveToEeprom(this->GetMqttBrokerHost(), MqttBrokerHostStartAddr, MqttBrokerHostLength);
    saveToEeprom(this->GetMqttCommandTopicBase(), MqttCommandTopicBaseStartAddr, MqttCommandTopicBaseLength);
    saveToEeprom(this->GetMqttStateTopicBase(), MqttStateTopicBaseStartAddr, MqttStateTopicBaseLength);
    saveToEeprom(this->GetMqttRetain() ? "1" : "0", MqttRetainStartAddr, MqttRetainLength);
    saveToEeprom(this->GetMqttPayloadOn(), MqttPayloadOnStartAddr, MqttPayloadOnLength);
    saveToEeprom(this->GetMqttPayloadOff(), MqttPayloadOffStartAddr, MqttPayloadOffLength);
    saveToEeprom(this->GetMqttWeatherTopic(), MqttWeatherTopicStartAddr, MqttWeatherTopicLength);

    char mqttBrokerPortStr[12];
    sprintf(mqttBrokerPortStr, "%d", this->GetMqttBrokerPort());
    saveToEeprom(mqttBrokerPortStr, MqttBrokerPortStartAddr, MqttBrokerPortLength);
}

void Settings::DumpToSerial() {
    Serial.print("MqttBrokerHost: ");       Serial.println(this->GetMqttBrokerHost());
    Serial.print("MqttBrokerPort: ");       Serial.println(this->GetMqttBrokerPort());
    Serial.print("MqttCommandTopicBase: "); Serial.println(this->GetMqttCommandTopicBase());
    Serial.print("MqttStateTopicBase: ");   Serial.println(this->GetMqttStateTopicBase());
    Serial.print("MqttWeatherTopic: ");     Serial.println(this->GetMqttWeatherTopic());
    Serial.print("MqttRetain: ");           Serial.println(this->GetMqttRetain());
    Serial.print("MqttPayloadOn: ");        Serial.println(this->GetMqttPayloadOn());
    Serial.print("MqttPayloadOff: ");       Serial.println(this->GetMqttPayloadOff());
}

void Settings::SetMqttBrokerHost(const char* value) {
    strcpy(this->MqttBrokerHost, value);
}

char* Settings::GetMqttBrokerHost() {
    return this->MqttBrokerHost;
}

void Settings::SetMqttBrokerPort(uint16_t value) {
    this->MqttBrokerPort = value;
}
void Settings::SetMqttBrokerPort(const char* value) {
    int mqttBrokerPort;
    sscanf(value, "%d", &mqttBrokerPort);
    this->SetMqttBrokerPort(mqttBrokerPort);
}
uint16_t Settings::GetMqttBrokerPort() {
    return this->MqttBrokerPort;
}
void Settings::SetMqttCommandTopicBase(const char* value) {
    strcpy(this->MqttCommandTopicBase, value);
}
char* Settings::GetMqttCommandTopicBase() {
    return this->MqttCommandTopicBase;
}
void Settings::SetMqttStateTopicBase(const char* value) {
    strcpy(this->MqttStateTopicBase, value);
}
char* Settings::GetMqttStateTopicBase() {
    return this->MqttStateTopicBase;
}
void Settings::SetMqttRetain(bool value) {
    this->MqttRetain = value;
}
void Settings::SetMqttRetain(const char* value) {
    this->MqttRetain = (strlen(value) > 0 ? "1" : "0");
}
bool Settings::GetMqttRetain() {
    return this->MqttRetain;
}
void Settings::SetMqttPayloadOn(const char* value) {
    strcpy(this->MqttPayloadOn, value);
}
char* Settings::GetMqttPayloadOn() {
    return this->MqttPayloadOn;
}
void Settings::SetMqttPayloadOff(const char* value) {
    strcpy(this->MqttPayloadOff, value);
}
char* Settings::GetMqttPayloadOff() {
    return this->MqttPayloadOff;
}
void Settings::SetMqttWeatherTopic(const char* value) {
    strcpy(this->MqttWeatherTopic, value);
}
char* Settings::GetMqttWeatherTopic() {
    return this->MqttWeatherTopic;
}
