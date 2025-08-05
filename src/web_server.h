#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>

// Forward declaration
class ConfigManager;

class WebServerManager {
private:
    WebServer* server;
    bool isInitialized;
    bool isConfigMode;
    ConfigManager* configManager;

public:
    WebServerManager();
    ~WebServerManager();
    
    void begin();
    void beginConfigMode();
    void setConfigManager(ConfigManager* cm);
    void update();
    void handleRoot();
    void handleConfig();
    void handleSaveConfig();
    void handleOTA();
    void handleStatus();
    void handleNotFound();
    bool isServerReady();
    void setConfigMode(bool mode);
    
private:
    String getStatusJSON();
};

#endif // WEB_SERVER_H 