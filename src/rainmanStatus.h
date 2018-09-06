#ifndef __RAINMANSTATUS_H__
#define __RAINMANSTATUS_H__

enum t_RainmanConnectionStatus {
    ConnectionOK,
    ConnectionNOK
};

class RainmanStatus {
    public:
    RainmanStatus();
    t_RainmanConnectionStatus GetWifiStatus();
    t_RainmanConnectionStatus GetMqttStatus();
    void SetWifiStatus(t_RainmanConnectionStatus status);
    void SetMqttStatus(t_RainmanConnectionStatus status);

    private:
    t_RainmanConnectionStatus wifiStatus;
    t_RainmanConnectionStatus mqttStatus;
};

#endif