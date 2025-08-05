// Глобальні змінні
let statusUpdateInterval;
let isUpdating = false;

// Ініціалізація при завантаженні сторінки
document.addEventListener('DOMContentLoaded', function() {
    console.log('ESP32 OTA Dashboard loaded');
    addLogEntry('Сторінка завантажена', 'info');
    
    // Завантаження початкового статусу
    refreshStatus();
    
    // Налаштування автоматичного оновлення кожні 10 секунд
    statusUpdateInterval = setInterval(refreshStatus, 10000);
});

// Функція для оновлення статусу
async function refreshStatus() {
    try {
        const response = await fetch('/status');
        if (response.ok) {
            const data = await response.json();
            updateStatusDisplay(data);
        } else {
            throw new Error('HTTP ' + response.status);
        }
    } catch (error) {
        console.error('Error fetching status:', error);
        addLogEntry('Помилка отримання статусу: ' + error.message, 'error');
        showErrorStatus();
    }
}

// Функція для оновлення відображення статусу
function updateStatusDisplay(data) {
    document.getElementById('version').textContent = data.version || 'Невідомо';
    document.getElementById('ip').textContent = data.ip || 'Не підключено';
    document.getElementById('wifi-status').textContent = data.wifi_status || 'Невідомо';
    document.getElementById('uptime').textContent = formatUptime(data.uptime || 0);
    
    // Оновлення кольору статусу Wi-Fi
    const wifiStatusElement = document.getElementById('wifi-status');
    if (data.wifi_status === 'connected') {
        wifiStatusElement.style.color = '#28a745';
    } else {
        wifiStatusElement.style.color = '#dc3545';
    }
}

// Функція для показу помилки статусу
function showErrorStatus() {
    document.getElementById('version').textContent = 'Помилка';
    document.getElementById('ip').textContent = 'Помилка';
    document.getElementById('wifi-status').textContent = 'Помилка';
    document.getElementById('uptime').textContent = 'Помилка';
}

// Функція для форматування часу роботи
function formatUptime(seconds) {
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const secs = seconds % 60;
    
    if (hours > 0) {
        return `${hours}г ${minutes}хв ${secs}с`;
    } else if (minutes > 0) {
        return `${minutes}хв ${secs}с`;
    } else {
        return `${secs}с`;
    }
}

// Функція для запуску OTA оновлення
async function startOTA() {
    if (isUpdating) {
        addLogEntry('Оновлення вже виконується', 'warning');
        return;
    }
    
    if (!confirm('Ви впевнені, що хочете запустити OTA оновлення? Це може зайняти кілька хвилин.')) {
        return;
    }
    
    isUpdating = true;
    const updateBtn = document.getElementById('update-btn');
    updateBtn.disabled = true;
    updateBtn.textContent = '🔄 Оновлення...';
    
    addLogEntry('Запуск OTA оновлення...', 'info');
    
    try {
        const response = await fetch('/ota', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        
        if (response.ok) {
            const result = await response.json();
            addLogEntry('OTA оновлення ініційовано: ' + result.status, 'success');
        } else {
            throw new Error('HTTP ' + response.status);
        }
    } catch (error) {
        console.error('Error starting OTA:', error);
        addLogEntry('Помилка запуску OTA: ' + error.message, 'error');
    } finally {
        isUpdating = false;
        updateBtn.disabled = false;
        updateBtn.textContent = '🔄 Запустити OTA оновлення';
    }
}

// Функція для додавання запису в лог
function addLogEntry(message, type = 'info') {
    const logContainer = document.getElementById('log');
    const logEntry = document.createElement('div');
    logEntry.className = 'log-entry';
    
    const now = new Date();
    const timeString = now.toLocaleTimeString();
    
    let typeClass = '';
    switch (type) {
        case 'success':
            typeClass = 'log-success';
            break;
        case 'error':
            typeClass = 'log-error';
            break;
        case 'warning':
            typeClass = 'log-warning';
            break;
        default:
            typeClass = 'log-message';
    }
    
    logEntry.innerHTML = `
        <span class="log-time">[${timeString}]</span>
        <span class="${typeClass}">${message}</span>
    `;
    
    logContainer.appendChild(logEntry);
    
    // Автоматичне прокручування до кінця
    logContainer.scrollTop = logContainer.scrollHeight;
    
    // Обмеження кількості записів у лозі (максимум 50)
    while (logContainer.children.length > 50) {
        logContainer.removeChild(logContainer.firstChild);
    }
}

// Функція для ручного оновлення статусу
function manualRefreshStatus() {
    addLogEntry('Оновлення статусу...', 'info');
    refreshStatus();
}

// Обробка помилок мережі
window.addEventListener('online', function() {
    addLogEntry('Мережеве з\'єднання відновлено', 'success');
});

window.addEventListener('offline', function() {
    addLogEntry('Мережеве з\'єднання втрачено', 'error');
});

// Очищення інтервалу при закритті сторінки
window.addEventListener('beforeunload', function() {
    if (statusUpdateInterval) {
        clearInterval(statusUpdateInterval);
    }
}); 