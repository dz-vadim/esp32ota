# ESP32 OTA + LCD1602 + Telegram Bot + WebUI

Проєкт для ESP32 з підтримкою:

* OTA-оновлення з GitHub
* Виводу статусу на LCD1602 I2C
* Керування через Telegram-бота
* Вебпанелі (WebUI) для перегляду статусу та OTA

---

## 📂 Структура проєкту

```
esp32-ota-lcd-web/
├── include/
│   └── config.h                # Налаштування Wi-Fi, Telegram, OTA
├── src/
│   ├── main.cpp                 # Головний файл
│   ├── lcd_manager.cpp/.h       # Робота з LCD
│   ├── wifi_manager.cpp/.h      # Wi-Fi логіка
│   ├── telegram_bot.cpp/.h      # Обробка команд бота
│   ├── ota_updater.cpp/.h       # OTA-оновлення
│   ├── web_server.cpp/.h        # Вебсервер
├── data/
│   ├── index.html               # Вебінтерфейс
│   ├── style.css
│   └── script.js
├── platformio.ini               # Налаштування PlatformIO
└── README.md
```

---

## ⚙️ Налаштування

1. Встановіть **PlatformIO** у Visual Studio Code.
2. Створіть новий проєкт:

   * **Board:** `esp32dev` або `ESP32 Dev Module`
   * **Framework:** `Arduino`
3. Замініть `platformio.ini` на:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
board_build.filesystem = spiffs

lib_deps =
  bokeh/FastBot @ ^1.1.0
  milesburton/LiquidCrystal_I2C @ ^1.1.4
  bblanchon/ArduinoJson @ ^6.21.3
```

4. Заповніть `include/config.h` своїми налаштуваннями:

```cpp
#define WIFI_SSID "YourWiFi"
#define WIFI_PASSWORD "YourPass"
#define BOT_TOKEN "123456:ABC..."
#define CHAT_ID "123456"
#define OTA_URL "https://raw.githubusercontent.com/user/repo/main/firmware.bin"
#define FW_VERSION "v1.0.0"
```

---

## 🚀 Запуск

1. **Збірка та прошивка:**

```bash
pio run --target upload
```

2. **Завантаження WebUI у SPIFFS:**

```bash
pio run --target uploadfs
```

3. Відкрийте **Serial Monitor**, щоб побачити IP-адресу ESP32:

```bash
pio device monitor
```

---

## 📱 Telegram-бот

Доступні команди:

* `/status` — показує IP та версію
* `/version` — поточна версія прошивки
* `/update` — запускає OTA-оновлення

---

## 🌐 WebUI

Відкрийте в браузері:

```
http://<ESP_IP>
```

Доступно:

* Перегляд IP та версії
* Кнопка для запуску OTA (у майбутньому можна розширити)

---

## 🔧 Підключення LCD1602

| ESP32 | LCD1602 I2C |
|-------|-------------|
| 3.3V  | VCC         |
| GND   | GND         |
| GPIO21| SDA         |
| GPIO22| SCL         |

---

## 📋 Функції

### LCD Дисплей
- Показує статус системи
- Відображає IP-адресу
- Показує версію прошивки
- Статус OTA-оновлення

### Wi-Fi Менеджер
- Автоматичне підключення
- Перепідключення при втраті з'єднання
- Моніторинг статусу

### Telegram Бот
- Команди для керування
- Сповіщення про статус
- Інформація про оновлення

### OTA Оновлення
- Завантаження з URL
- Перевірка доступності оновлень
- Безпечне оновлення прошивки

### Вебсервер
- REST API для статусу
- Вебінтерфейс
- SPIFFS для файлів

---

## 🛠️ Розробка

### Додавання нових команд Telegram

Додайте в `telegram_bot.cpp`:

```cpp
else if (message == "/newcommand") {
    bot->sendMessage(chatId.c_str(), "New command response");
}
```

### Розширення WebUI

Додайте нові ендпоінти в `web_server.cpp`:

```cpp
server->on("/newendpoint", HTTP_GET, [this]() { 
    handleNewEndpoint(); 
});
```

---

## 📝 Ліцензія

MIT License

---

## 🤝 Внесок

1. Fork проєкт
2. Створіть feature branch
3. Commit зміни
4. Push до branch
5. Створіть Pull Request 