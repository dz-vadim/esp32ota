#include "telegram_bot.h"
#include "config.h"

TelegramBot::TelegramBot() {
    bot = new FastBot();
    isInitialized = false;
}

TelegramBot::~TelegramBot() {
    if (bot) {
        delete bot;
    }
}

void TelegramBot::begin(const String& token, const String& chatId) {
    this->token = token;
    this->chatId = chatId;
    
    bot->setToken(token.c_str());
    isInitialized = true;
    
    Serial.println("Telegram bot initialized");
}

void TelegramBot::setMessageHandler(void (*handler)(FB_msg&)) {
    if (isInitialized && bot) {
        bot->attach(handler);
    }
}

void TelegramBot::update() {
    if (isInitialized) {
        bot->tick();
    }
}

void TelegramBot::sendMessage(const String& message) {
    if (isInitialized) {
        bot->sendMessage(chatId.c_str(), message.c_str());
    }
}

void TelegramBot::sendStatus() {
    if (isInitialized) {
        String status = "🔄 ESP32 Status:\n";
        status += "📶 WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "\n";
        status += "🌐 IP: " + WiFi.localIP().toString() + "\n";
        status += "📱 Version: " + String(FW_VERSION);
        
        sendMessage(status);
    }
}

void TelegramBot::sendVersion() {
    if (isInitialized) {
        String version = "📱 Current Version: " + String(FW_VERSION);
        sendMessage(version);
    }
}

void TelegramBot::handleCommands() {
    if (isInitialized) {
        // Обробка команд буде виконуватися через callback функцію
        // Ця функція залишається для внутрішнього використання
    }
}

bool TelegramBot::isBotReady() {
    return isInitialized;
}

// Глобальна функція для обробки повідомлень
// Буде реалізована в main.cpp 