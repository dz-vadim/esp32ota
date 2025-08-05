#include "ota_updater.h"
#include "config.h"

OTAUpdater::OTAUpdater() {
    isUpdating = false;
    updateStartTime = 0;
    updateCallback = nullptr;
}

OTAUpdater::~OTAUpdater() {
    // Деструктор
}

void OTAUpdater::begin(const String& url, const String& version) {
    this->otaUrl = url;
    this->currentVersion = version;
}

bool OTAUpdater::checkForUpdate() {
    HTTPClient http;
    http.begin(otaUrl);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        if (contentLength > 0) {
            Serial.println("Update available. Size: " + String(contentLength));
            http.end();
            return true;
        }
    }
    
    http.end();
    return false;
}

bool OTAUpdater::performUpdate() {
    if (isUpdating) {
        return false;
    }
    
    isUpdating = true;
    updateStartTime = millis();
    
    HTTPClient http;
    http.begin(otaUrl);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        
        if (contentLength > 0) {
            if (Update.begin(contentLength)) {
                size_t written = Update.writeStream(http.getStream());
                
                if (written == contentLength) {
                    if (Update.end()) {
                        Serial.println("Update successful!");
                        if (updateCallback) {
                            updateCallback("Update successful!");
                        }
                        http.end();
                        return true;
                    } else {
                        Serial.println("Update failed!");
                        if (updateCallback) {
                            updateCallback("Update failed!");
                        }
                    }
                } else {
                    Serial.println("Update failed: written != contentLength");
                    if (updateCallback) {
                        updateCallback("Update failed: written != contentLength");
                    }
                }
            } else {
                Serial.println("Update failed: not enough space");
                if (updateCallback) {
                    updateCallback("Update failed: not enough space");
                }
            }
        }
    } else {
        Serial.println("HTTP request failed: " + String(httpCode));
        if (updateCallback) {
            updateCallback("HTTP request failed: " + String(httpCode));
        }
    }
    
    http.end();
    isUpdating = false;
    return false;
}

bool OTAUpdater::isUpdateInProgress() {
    return isUpdating;
}

String OTAUpdater::getCurrentVersion() {
    return currentVersion;
}

void OTAUpdater::setUpdateCallback(void (*callback)(const String&)) {
    this->updateCallback = callback;
} 