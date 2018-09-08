#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <EEPROM.h>
#include <Arduino.h>

// String MqttBrokerHost;          //35 bytes | start 0, length 35
// String MqttBrokerPort;          //8 bytes  | start 36, length 8
// String MqttCommandTopicBase;    //35 bytes | start 44, length 35
// String MqttStateTopicBase;      //35 bytes | start 79, length 35
// String MqttRetain;              //5 bytes  | start 114, length 5
// String MqttPayloadOn;           //10 bytes | start 119, length 10
// String MqttPayloadOff;          //10 bytes | start 129, length 10
// String MqttWeatherTopic         //35 bytes | start 139, length 35
//Total nr of bytes: 174

#define MAX_EEPROM_SIZE 512

#define MqttBrokerHostStartAddr         0
#define MqttBrokerPortStartAddr         36
#define MqttCommandTopicBaseStartAddr   44
#define MqttStateTopicBaseStartAddr     79
#define MqttRetainStartAddr             114
#define MqttPayloadOnStartAddr          119
#define MqttPayloadOffStartAddr         129
#define MqttWeatherTopicStartAddr       139
#define MqttBrokerHostLength            35
#define MqttBrokerPortLength            8
#define MqttCommandTopicBaseLength      35
#define MqttStateTopicBaseLength        35
#define MqttRetainLength                5
#define MqttPayloadOnLength             10
#define MqttPayloadOffLength            10
#define MqttWeatherTopicLength          35

class Settings
{
    private:
    char MqttBrokerHost[MqttBrokerHostLength+1];
    uint16_t MqttBrokerPort;
    char MqttCommandTopicBase[MqttCommandTopicBaseLength+1];
    char MqttStateTopicBase[MqttStateTopicBaseLength+1];
    bool MqttRetain;
    char MqttPayloadOn[MqttPayloadOnLength+1];
    char MqttPayloadOff[MqttPayloadOffLength+1];
    char MqttWeatherTopic[MqttWeatherTopicLength+1];
    void ReadAllSettingsFromEeprom();

    public:
    Settings();
    ~Settings();
    void Begin();
    char* GetMqttBrokerHost();
    void SetMqttBrokerHost(const char* value);
    void SetMqttBrokerPort(uint16_t value);
    void SetMqttBrokerPort(const char* value);
    uint16_t GetMqttBrokerPort();
    void SetMqttCommandTopicBase(const char* value);
    char* GetMqttCommandTopicBase();
    void SetMqttStateTopicBase(const char* value);
    char* GetMqttStateTopicBase();
    void SetMqttRetain(bool value);
    void SetMqttRetain(const char* value);
    bool GetMqttRetain();
    void SetMqttPayloadOn(const char* value);
    char* GetMqttPayloadOn();
    void SetMqttPayloadOff(const char* value);
    char* GetMqttPayloadOff();
    char* GetMqttWeatherTopic();
    void SetMqttWeatherTopic(const char* value);
    
    void Save();
    void EraseAll();
    void DumpToSerial();
};

#endif