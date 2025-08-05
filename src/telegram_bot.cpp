#include "telegram_bot.h"
#include "config.h"

TelegramBot::TelegramBot() {
    bot = new FastBot();
    isInitialized = false;
    messageHandler = nullptr;
}

TelegramBot::~TelegramBot() {
    if (bot) {
        delete bot;
    }
}

void TelegramBot::begin(const String& token) {
    this->token = token;
    
    bot->setToken(token.c_str());
    isInitialized = true;
    
    Serial.println("Telegram bot initialized");
}

// Глобальна змінна для зберігання вказівника на екземпляр TelegramBot
static TelegramBot* currentBot = nullptr;

// Глобальна функція-обгортка
static void globalMessageHandler(FB_msg& msg) {
    if (currentBot) {
        currentBot->lastChatId = msg.chatID;  // Зберігаємо ID чату
        if (currentBot->messageHandler) {
            currentBot->messageHandler(msg);  // Викликаємо оригінальний обробник
        }
    }
}

void TelegramBot::setMessageHandler(void (*handler)(FB_msg&)) {
    if (isInitialized && bot) {
        messageHandler = handler;
        currentBot = this;
        bot->attach(globalMessageHandler);
    }
}

void TelegramBot::update() {
    if (isInitialized) {
        bot->tick();
    }
}

void TelegramBot::sendMessage(const String& message, const String& targetChatId) {
    if (isInitialized) {
        bot->sendMessage(targetChatId.c_str(), message.c_str());
    }
}

void TelegramBot::sendStatus() {
    if (isInitialized && lastChatId.length() > 0) {
        String status = "🔄 ESP32 Status:\n";
        status += "📶 WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "\n";
        status += "🌐 IP: " + WiFi.localIP().toString() + "\n";
        status += "📱 Version: " + String(FW_VERSION);
        
        sendMessage(status, lastChatId);
    }
}

void TelegramBot::sendVersion() {
    if (isInitialized && lastChatId.length() > 0) {
        String version = "📱 Current Version: " + String(FW_VERSION);
        sendMessage(version, lastChatId);
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