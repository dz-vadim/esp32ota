#include "lcd_manager.h"
#include "config.h"

LCDManager::LCDManager() {
    lcd = new LiquidCrystal_I2C(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
    lastUpdate = 0;
    isConnected = false;
}

LCDManager::~LCDManager() {
    if (lcd) {
        delete lcd;
    }
}

void LCDManager::begin() {
    Wire.begin();
    lcd->init();
    lcd->backlight();
    lcd->clear();
    isConnected = true;
    
    // Перевірка підключення
    Wire.beginTransmission(LCD_I2C_ADDRESS);
    isConnected = (Wire.endTransmission() == 0);
    
    if (isConnected) {
        printStatus("ESP32 Ready");
    } else {
        Serial.println("LCD not found!");
    }
}

void LCDManager::clear() {
    if (isConnected) {
        lcd->clear();
    }
}

void LCDManager::setCursor(uint8_t col, uint8_t row) {
    if (isConnected) {
        lcd->setCursor(col, row);
    }
}

void LCDManager::print(const String& text) {
    if (isConnected) {
        lcd->print(text);
    }
}

void LCDManager::printStatus(const String& status) {
    if (isConnected) {
        clear();
        setCursor(0, 0);
        // Обрізаємо текст до 16 символів
        String shortStatus = status;
        if (shortStatus.length() > 16) {
            shortStatus = shortStatus.substring(0, 16);
        }
        print(shortStatus);
    }
}

void LCDManager::printIP(const String& ip) {
    if (isConnected) {
        clear();
        setCursor(0, 0);
        print("IP:" + ip);
        setCursor(0, 1);
        print("WiFi OK");
    }
}

void LCDManager::printVersion(const String& version) {
    if (isConnected) {
        clear();
        setCursor(0, 0);
        print("Ver:" + version);
    }
}

void LCDManager::printOTAStatus(const String& status) {
    if (isConnected) {
        clear();
        setCursor(0, 0);
        print("OTA:" + status);
    }
}

void LCDManager::update() {
    // Оновлення дисплею кожні 5 секунд
    if (millis() - lastUpdate > 5000) {
        lastUpdate = millis();
        // Можна додати анімацію або інші ефекти
    }
} 