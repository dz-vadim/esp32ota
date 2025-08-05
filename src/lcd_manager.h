#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LCDManager {
private:
    LiquidCrystal_I2C* lcd;
    unsigned long lastUpdate;
    bool isConnected;

public:
    LCDManager();
    ~LCDManager();
    
    void begin();
    void clear();
    void setCursor(uint8_t col, uint8_t row);
    void print(const String& text);
    void printStatus(const String& status);
    void printIP(const String& ip);
    void printVersion(const String& version);
    void printOTAStatus(const String& status);
    void update();
};

#endif // LCD_MANAGER_H 