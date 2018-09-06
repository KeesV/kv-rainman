#ifndef __RAINMANSTATUS_H__
#define __RAINMANSTATUS_H__

enum t_RainmanConnectionStatus {
    ConnectionOK,
    ConnectionNOK
};

enum t_RainmanWeather {
    UNKNOWN,
    CLOUDY,
    FOG,
    HAIL,
    LIGHTNING,
    LIGHTNING_RAINY,
    PARTLYCLOUDY,
    POURING,
    RAINY,
    SNOWY,
    SNOWY_RAINY,
    SUNNY,
    WINDY,
    WINDY_VARIANT
};

class RainmanStatus {
    public:
    RainmanStatus();
    t_RainmanConnectionStatus GetWifiStatus();
    t_RainmanConnectionStatus GetMqttStatus();
    t_RainmanWeather GetWeather();
    void SetWifiStatus(t_RainmanConnectionStatus status);
    void SetMqttStatus(t_RainmanConnectionStatus status);
    void SetWeather(t_RainmanWeather weather);


    private:
    t_RainmanConnectionStatus wifiStatus;
    t_RainmanConnectionStatus mqttStatus;
    t_RainmanWeather currentWeather;
};

#endif