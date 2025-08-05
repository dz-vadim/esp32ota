#include "web_server.h"
#include "config.h"
#include "config_manager.h"
#include <ArduinoJson.h>

WebServerManager::WebServerManager() {
    server = new WebServer(HTTP_PORT);
    isInitialized = false;
    isConfigMode = false;
    configManager = nullptr;
}

WebServerManager::~WebServerManager() {
    if (server) {
        delete server;
    }
}

void WebServerManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // Налаштування обробників
    server->on("/", HTTP_GET, [this]() { handleRoot(); });
    server->on("/ota", HTTP_POST, [this]() { handleOTA(); });
    server->on("/status", HTTP_GET, [this]() { handleStatus(); });
    server->onNotFound([this]() { handleNotFound(); });
    
    server->begin();
    isInitialized = true;
    
    Serial.println("Web server started on port " + String(HTTP_PORT));
}

void WebServerManager::beginConfigMode() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // Налаштування обробників для режиму конфігурації
    server->on("/", HTTP_GET, [this]() { handleConfig(); });
    server->on("/save", HTTP_POST, [this]() { handleSaveConfig(); });
    server->onNotFound([this]() { handleNotFound(); });
    
    server->begin();
    isInitialized = true;
    isConfigMode = true;
    
    Serial.println("Config web server started on port " + String(HTTP_PORT));
}

void WebServerManager::update() {
    if (isInitialized) {
        server->handleClient();
    }
}

void WebServerManager::handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
        server->send(404, "text/plain", "File not found");
        return;
    }
    
    server->streamFile(file, "text/html");
    file.close();
}

void WebServerManager::handleOTA() {
    server->send(200, "application/json", "{\"status\":\"OTA update initiated\"}");
    // Тут буде виклик OTA оновлення
}

void WebServerManager::handleStatus() {
    server->send(200, "application/json", getStatusJSON());
}

void WebServerManager::handleNotFound() {
    server->send(404, "text/plain", "Not found");
}

bool WebServerManager::isServerReady() {
    return isInitialized;
}

void WebServerManager::setConfigMode(bool mode) {
    isConfigMode = mode;
}

void WebServerManager::setConfigManager(ConfigManager* cm) {
    configManager = cm;
}

void WebServerManager::handleConfig() {
    File file = SPIFFS.open("/config.html", "r");
    if (!file) {
        // Якщо файл не знайдено, показуємо простий HTML
        String html = "<html><head><title>ESP32 Config</title></head><body>";
        html += "<h1>ESP32 Configuration</h1>";
        html += "<p>Config page not found. Please upload config.html to SPIFFS.</p>";
        html += "</body></html>";
        server->send(200, "text/html", html);
        return;
    }
    
    server->streamFile(file, "text/html");
    file.close();
}

void WebServerManager::handleSaveConfig() {
    if (server->hasArg("ssid") && server->hasArg("password")) {
        String ssid = server->arg("ssid");
        String password = server->arg("password");
        
        if (configManager && configManager->saveWiFiConfig(ssid, password)) {
            server->send(200, "application/json", "{\"status\":\"success\",\"message\":\"Configuration saved\"}");
            
            // Перезавантаження ESP32 через 3 секунди
            delay(3000);
            ESP.restart();
        } else {
            server->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to save configuration\"}");
        }
    } else {
        server->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing parameters\"}");
    }
}

String WebServerManager::getStatusJSON() {
    JsonDocument doc;
    
    doc["version"] = FW_VERSION;
    doc["ip"] = WiFi.localIP().toString();
    doc["wifi_status"] = (WiFi.status() == WL_CONNECTED) ? "connected" : "disconnected";
    doc["uptime"] = millis() / 1000;
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
} 