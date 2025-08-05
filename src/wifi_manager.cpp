#include "wifi_manager.h"

WiFiManager::WiFiManager() {
    isConnected = false;
    lastReconnectAttempt = 0;
}

WiFiManager::~WiFiManager() {
    // Деструктор
}

void WiFiManager::begin(const String& ssid, const String& password) {
    this->ssid = ssid;
    this->password = password;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    
    Serial.println("Connecting to WiFi...");
}

bool WiFiManager::connect() {
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        isConnected = true;
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        isConnected = false;
        Serial.println();
        Serial.println("WiFi connection failed!");
        return false;
    }
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    isConnected = false;
}

bool WiFiManager::isWiFiConnected() {
    isConnected = (WiFi.status() == WL_CONNECTED);
    return isConnected;
}

String WiFiManager::getLocalIP() {
    if (isWiFiConnected()) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

void WiFiManager::update() {
    // Перевірка з'єднання кожні 30 секунд
    if (millis() - lastReconnectAttempt > 30000) {
        lastReconnectAttempt = millis();
        reconnectIfNeeded();
    }
}

void WiFiManager::reconnectIfNeeded() {
    if (!isWiFiConnected()) {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.reconnect();
    }
} 