#include "config_manager.h"
#include "config.h"

ConfigManager::ConfigManager() {
    isConfigMode = false;
    configStartTime = 0;
    wifiConfig.isValid = false;
    telegramConfig.isValid = false;
}

ConfigManager::~ConfigManager() {
    // Деструктор
}

void ConfigManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // Завантаження збережених налаштувань
    loadWiFiConfig();
    loadTelegramConfig();
    
    Serial.println("ConfigManager initialized");
}

bool ConfigManager::loadWiFiConfig() {
    if (!SPIFFS.exists(WIFI_CONFIG_FILE)) {
        Serial.println("WiFi config file not found");
        return false;
    }
    
    File file = SPIFFS.open(WIFI_CONFIG_FILE, "r");
    if (!file) {
        Serial.println("Failed to open WiFi config file");
        return false;
    }
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.println("Failed to parse WiFi config");
        return false;
    }
    
    wifiConfig.ssid = doc["ssid"].as<String>();
    wifiConfig.password = doc["password"].as<String>();
    wifiConfig.isValid = !wifiConfig.ssid.isEmpty() && !wifiConfig.password.isEmpty();
    
    Serial.println("WiFi config loaded: " + wifiConfig.ssid);
    return wifiConfig.isValid;
}

bool ConfigManager::loadTelegramConfig() {
    if (!SPIFFS.exists(TELEGRAM_CONFIG_FILE)) {
        Serial.println("Telegram config file not found");
        return false;
    }
    
    File file = SPIFFS.open(TELEGRAM_CONFIG_FILE, "r");
    if (!file) {
        Serial.println("Failed to open Telegram config file");
        return false;
    }
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.println("Failed to parse Telegram config");
        return false;
    }
    
    telegramConfig.botToken = doc["token"].as<String>();
    telegramConfig.chatId = doc["chat_id"].as<String>();
    telegramConfig.isValid = !telegramConfig.botToken.isEmpty() && !telegramConfig.chatId.isEmpty();
    
    Serial.println("Telegram config loaded");
    return telegramConfig.isValid;
}

bool ConfigManager::saveWiFiConfig(const String& ssid, const String& password) {
    JsonDocument doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    
    File file = SPIFFS.open(WIFI_CONFIG_FILE, "w");
    if (!file) {
        Serial.println("Failed to create WiFi config file");
        return false;
    }
    
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write WiFi config");
        file.close();
        return false;
    }
    
    file.close();
    
    wifiConfig.ssid = ssid;
    wifiConfig.password = password;
    wifiConfig.isValid = true;
    
    Serial.println("WiFi config saved: " + ssid);
    return true;
}

bool ConfigManager::saveTelegramConfig(const String& token, const String& chatId) {
    JsonDocument doc;
    doc["token"] = token;
    doc["chat_id"] = chatId;
    
    File file = SPIFFS.open(TELEGRAM_CONFIG_FILE, "w");
    if (!file) {
        Serial.println("Failed to create Telegram config file");
        return false;
    }
    
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write Telegram config");
        file.close();
        return false;
    }
    
    file.close();
    
    telegramConfig.botToken = token;
    telegramConfig.chatId = chatId;
    telegramConfig.isValid = true;
    
    Serial.println("Telegram config saved");
    return true;
}

void ConfigManager::clearConfigs() {
    if (SPIFFS.exists(WIFI_CONFIG_FILE)) {
        SPIFFS.remove(WIFI_CONFIG_FILE);
    }
    if (SPIFFS.exists(TELEGRAM_CONFIG_FILE)) {
        SPIFFS.remove(TELEGRAM_CONFIG_FILE);
    }
    
    wifiConfig.isValid = false;
    telegramConfig.isValid = false;
    
    Serial.println("All configs cleared");
}

WiFiConfig ConfigManager::getWiFiConfig() {
    return wifiConfig;
}

TelegramConfig ConfigManager::getTelegramConfig() {
    return telegramConfig;
}

bool ConfigManager::isInConfigMode() {
    return isConfigMode;
}

void ConfigManager::enterConfigMode() {
    isConfigMode = true;
    configStartTime = millis();
    Serial.println("Entered config mode");
}

void ConfigManager::exitConfigMode() {
    isConfigMode = false;
    Serial.println("Exited config mode");
}

bool ConfigManager::isConfigTimeout() {
    return isConfigMode && (millis() - configStartTime > CONFIG_TIMEOUT);
}

bool ConfigManager::hasValidWiFiConfig() {
    return wifiConfig.isValid;
}

bool ConfigManager::hasValidTelegramConfig() {
    return telegramConfig.isValid;
} 