#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi налаштування
#define WIFI_SSID "YourWiFi"
#define WIFI_PASSWORD "YourPass"

// Telegram Bot налаштування
#define BOT_TOKEN "8452307846:AAFndCi2IKpibaWtSZMQr43N8Cv3eZQ0p1Q"
#define CHAT_ID "123456"

// OTA налаштування
#define OTA_URL "https://raw.githubusercontent.com/dz-vadim/esp32ota/main/bin/firmware.bin"
#define FW_VERSION "v1.0.0"

// LCD налаштування
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// HTTP сервер
#define HTTP_PORT 80

// Налаштування режиму конфігурації
#define CONFIG_MODE_SSID "ESP32_Config"
#define CONFIG_MODE_PASSWORD "12345678"
#define CONFIG_TIMEOUT 300000 // 5 хвилин

// Файли для збереження налаштувань
#define WIFI_CONFIG_FILE "/wifi_config.json"
#define TELEGRAM_CONFIG_FILE "/telegram_config.json"

#endif // CONFIG_H 