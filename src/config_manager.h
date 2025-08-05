#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

struct WiFiConfig {
    String ssid;
    String password;
    bool isValid;
};

struct TelegramConfig {
    String botToken;
    String chatId;
    bool isValid;
};

class ConfigManager {
private:
    WiFiConfig wifiConfig;
    TelegramConfig telegramConfig;
    bool isConfigMode;
    unsigned long configStartTime;

public:
    ConfigManager();
    ~ConfigManager();
    
    void begin();
    bool loadWiFiConfig();
    bool loadTelegramConfig();
    bool saveWiFiConfig(const String& ssid, const String& password);
    bool saveTelegramConfig(const String& token, const String& chatId);
    void clearConfigs();
    
    WiFiConfig getWiFiConfig();
    TelegramConfig getTelegramConfig();
    
    bool isInConfigMode();
    void enterConfigMode();
    void exitConfigMode();
    bool isConfigTimeout();
    
    bool hasValidWiFiConfig();
    bool hasValidTelegramConfig();
};

#endif // CONFIG_MANAGER_H 