#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <FastBot.h>
#include <Arduino.h>

class TelegramBot {
private:
    String token;
    bool isInitialized;

public:
    FastBot* bot; // Публічний доступ для обробки повідомлень

public:
    TelegramBot();
    ~TelegramBot();
    
    void begin(const String& token);
    void setMessageHandler(void (*handler)(FB_msg&));
    void update();
    void sendMessage(const String& message, const String& targetChatId = "");
    void sendStatus();
    void sendVersion();
    void handleCommands();
    bool isBotReady();
};

#endif // TELEGRAM_BOT_H 