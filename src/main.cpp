#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "config.h"
#include "lcd_manager.h"
#include "wifi_manager.h"
#include "telegram_bot.h"
#include "ota_updater.h"
#include "web_server.h"
#include "config_manager.h"
#include <FastBot.h>

// Глобальні об'єкти
LCDManager lcdManager;
WiFiManager wifiManager;
TelegramBot telegramBot;
OTAUpdater otaUpdater;
WebServerManager webServer;
ConfigManager configManager;

// Глобальні змінні
unsigned long lastStatusUpdate = 0;
bool systemReady = false;

// Декларація функцій
void enterConfigMode();
void clearConfigs();

// Глобальна функція для обробки повідомлень Telegram
void sendNewMessage(FB_msg& msg) {
    String message = msg.text;
    String chatId = msg.chatID;
    
    if (message == "/start") {
        telegramBot.bot->sendMessage(chatId.c_str(), "🤖 ESP32 Bot started!\nUse /status, /version, /update");
    }
    else if (message == "/status") {
        telegramBot.sendStatus();
    }
    else if (message == "/version") {
        telegramBot.sendVersion();
    }
    else if (message == "/update") {
        telegramBot.bot->sendMessage(chatId.c_str(), "🔄 Starting OTA update...");
        // Тут буде виклик OTA оновлення
    }
    else if (message == "/clear") {
        telegramBot.bot->sendMessage(chatId.c_str(), "🗑️ Clearing all configurations...");
        clearConfigs();
    }
    else {
        telegramBot.bot->sendMessage(chatId.c_str(), "❓ Unknown command. Use /status, /version, /update, /clear");
    }
}

// Функція зворотного виклику для OTA
void onOTAUpdate(const String& status) {
    lcdManager.printOTAStatus(status);
    telegramBot.sendMessage("OTA Update: " + status);
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 OTA + LCD + Telegram + WebUI Starting...");
    
    // Ініціалізація ConfigManager
    configManager.begin();
    
    // Ініціалізація LCD
    lcdManager.begin();
    lcdManager.printStatus("Starting");
    
    // Перевірка наявності збережених налаштувань Wi-Fi
    if (configManager.hasValidWiFiConfig()) {
        // Використовуємо збережені налаштування
        WiFiConfig wifiConfig = configManager.getWiFiConfig();
        wifiManager.begin(wifiConfig.ssid, wifiConfig.password);
        
        if (wifiManager.connect()) {
            lcdManager.printIP(wifiManager.getLocalIP());
            systemReady = true;
            

            
            // Ініціалізація Telegram бота
            telegramBot.begin(BOT_TOKEN, CHAT_ID);
            telegramBot.setMessageHandler(sendNewMessage);
            
            // Ініціалізація OTA
            otaUpdater.begin(OTA_URL, FW_VERSION);
            otaUpdater.setUpdateCallback(onOTAUpdate);
            
            // Ініціалізація вебсервера
            webServer.setConfigManager(&configManager);
            webServer.begin();
            
            // Відправка повідомлення про запуск
            telegramBot.sendMessage("🚀 ESP32 started successfully!\nIP: " + wifiManager.getLocalIP() + "\nVersion: " + FW_VERSION);
            

            
            Serial.println("System initialized successfully!");
        } else {
            // Помилка підключення - переходимо в режим налаштування
            enterConfigMode();
        }
    } else {
        // Немає збережених налаштувань - переходимо в режим налаштування
        enterConfigMode();
    }
}

void enterConfigMode() {
    Serial.println("Entering configuration mode...");
    lcdManager.printStatus("Config");
    lcdManager.printIP("192.168.4.1");
    
    // Запуск точки доступу
    WiFi.mode(WIFI_AP);
    WiFi.softAP(CONFIG_MODE_SSID, CONFIG_MODE_PASSWORD);
    
    // Ініціалізація вебсервера в режимі налаштування
    webServer.setConfigManager(&configManager);
    webServer.beginConfigMode();
    
    Serial.println("Configuration mode started");
    Serial.println("SSID: " + String(CONFIG_MODE_SSID));
    Serial.println("Password: " + String(CONFIG_MODE_PASSWORD));
    Serial.println("IP: 192.168.4.1");
}

void clearConfigs() {
    configManager.clearConfigs();
    Serial.println("All configurations cleared");
    lcdManager.printStatus("Config Cleared");
    delay(2000);
    ESP.restart();
}

void loop() {
            // Оновлення всіх модулів
        if (systemReady) {
            wifiManager.update();
            telegramBot.update();
            webServer.update();
            lcdManager.update();
        
        // Оновлення статусу кожні 30 секунд
        if (millis() - lastStatusUpdate > 30000) {
            lastStatusUpdate = millis();
            
            if (wifiManager.isWiFiConnected()) {
                // Перевірка на оновлення кожні 5 хвилин
                static unsigned long lastUpdateCheck = 0;
                if (millis() - lastUpdateCheck > 300000) { // 5 хвилин
                    lastUpdateCheck = millis();
                    
                    if (otaUpdater.checkForUpdate()) {
                        telegramBot.sendMessage("🔄 New firmware update available!");
                        lcdManager.printOTAStatus("Update Available");
                    }
                }
            }
        }
    } else {
        // Режим налаштування
        webServer.update();
        
        // Показуємо IP налаштувань на LCD
        static unsigned long lastLCDUpdate = 0;
        if (millis() - lastLCDUpdate > 5000) {
            lastLCDUpdate = millis();
            lcdManager.printStatus("Config");
            lcdManager.printIP("192.168.4.1");
        }
    }
    
    delay(100); // Невелика затримка для стабільності
} 