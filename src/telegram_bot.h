#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <FastBot.h>
#include <Arduino.h>

class TelegramBot {
private:
    String token;
    String chatId;
    bool isInitialized;

public:
    FastBot* bot; // Публічний доступ для обробки повідомлень

public:
    TelegramBot();
    ~TelegramBot();
    
    void begin(const String& token, const String& chatId);
    void setMessageHandler(void (*handler)(FB_msg&));
    void update();
    void sendMessage(const String& message);
    void sendStatus();
    void sendVersion();
    void handleCommands();
    bool isBotReady();
};

#endif // TELEGRAM_BOT_H 