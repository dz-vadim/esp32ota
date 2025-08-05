#ifndef OTA_UPDATER_H
#define OTA_UPDATER_H

#include <HTTPClient.h>
#include <Update.h>
#include <Arduino.h>

class OTAUpdater {
private:
    String otaUrl;
    String currentVersion;
    bool isUpdating;
    unsigned long updateStartTime;

public:
    OTAUpdater();
    ~OTAUpdater();
    
    void begin(const String& url, const String& version);
    bool checkForUpdate();
    bool performUpdate();
    bool isUpdateInProgress();
    String getCurrentVersion();
    void setUpdateCallback(void (*callback)(const String&));
    
private:
    void (*updateCallback)(const String&);
};

#endif // OTA_UPDATER_H 