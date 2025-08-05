#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WiFiManager {
private:
    String ssid;
    String password;
    bool isConnected;
    unsigned long lastReconnectAttempt;

public:
    WiFiManager();
    ~WiFiManager();
    
    void begin(const String& ssid, const String& password);
    bool connect();
    void disconnect();
    bool isWiFiConnected();
    String getLocalIP();
    void update();
    void reconnectIfNeeded();
};

#endif // WIFI_MANAGER_H 