/*
 * esp32_gps_ntp.ino
 * Główny plik projektu ESP32 GPS NTP Server v04.01
 *
 * Architektura:
 *   TinyGPS++ -> gps.cpp/gps.h (centralny moduł, globalna struktura gpsData)
 *                  |
 *                  +-- oled.cpp   (wyświetlacz)
 *                  +-- ntp.cpp    (serwer NTP Stratum 1)
 *                  +-- web.cpp    (strona statusu HTML)
 *
 * Założenia:
 *  - Arduino IDE 1.8.x
 *  - ESP32 WROOM Mini D1
 *  - OLED SSD1306 64x48
 *  - GPS GY-NEO6MV2
 *  - Przycisk SW1 na GPIO0
 *  - Stratum 1 NTP zgodny z RFC 5905
 *  - Kod proceduralny, bez klas C++ (poza bibliotekami)
 *  - Jedna globalna struktura GPS_DATA (gpsData)
 *  - Przygotowane pod przyszłą obsługę RTC i PPS
 */

#include "config.h"
#include "gps.h"
#include "oled.h"
#include "ntp.h"
#include "web.h"

#include <WiFi.h>
#include <WiFiManager.h>

// --- ZMIENNE POMOCNICZE ---
static uint32_t lastDisplayRefresh = 0;
static uint32_t lastButtonCheck    = 0;

// --- KONFIGURACJA WIFI PRZEZ WiFiManager ---
static void wifiInit(void) {
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(180);  // 3 minuty timeout portalu

    if (!wifiManager.autoConnect("ESP32-GPS-NTP")) {
        // Timeout - restart
        ESP.restart();
    }

    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());
}

// --- OBSŁUGA PRZYCISKU SW1 ---
static void buttonProcess(void) {
    uint32_t now = millis();
    if ((now - lastButtonCheck) < BUTTON_DEBOUNCE_MS) {
        return;
    }
    lastButtonCheck = now;

    // GPIO 0 ma wewnętrzny pull-up, przycisk zwiera do GND
    if (digitalRead(BUTTON_PIN) == LOW) {
        // Przełączenie trybu wyświetlania
        gpsData.displayMode = (gpsData.displayMode + 1) % MODE_COUNT;
        gpsData.buttonPressed = true;

        // Czekamy na zwolnienie przycisku (prosty debounce)
        while (digitalRead(BUTTON_PIN) == LOW) {
            delay(10);
        }
    }
}

// ============================================================
//                        SETUP
// ============================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println(FW_NAME);
    Serial.print("Wersja: ");
    Serial.println(FW_VERSION);

    // Konfiguracja pinu przycisku
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Przygotowanie pod PPS (w przyszłych wersjach)
    if (PPS_ENABLED) {
        // TODO: pinMode(PPS_PIN, INPUT); + attachInterrupt()
    }

    // Inicjalizacja modułu GPS (czyści gpsData, uruchamia UART)
    gpsInit();

    // Inicjalizacja wyświetlacza
    oledInit();

    // Konfiguracja WiFi
    wifiInit();

    // Uruchomienie serwera NTP (UDP 123)
    ntpInit();
    Serial.print("Serwer NTP na porcie ");
    Serial.println(NTP_PORT);

    // Uruchomienie serwera WWW
    webInit();
    Serial.print("Serwer WWW na porcie ");
    Serial.println(WEB_PORT);
}

// ============================================================
//                         LOOP
// ============================================================
void loop() {
    uint32_t now = millis();

    // 1. Odczyt danych z modułu GPS
    gpsProcess();

    // 2. Synchronizacja zegara systemowego z GPS
    if (gpsShouldSync()) {
        if (gpsSyncSystemTime()) {
            Serial.print("Czas zsynchronizowany z GPS: ");
            char buf[24];
            gpsFormatTimeUTC(buf, sizeof(buf));
            Serial.println(buf);
        }
    }

    // 3. Obsługa przycisku SW1
    buttonProcess();

    // 4. Obsługa żądań NTP
    ntpProcess();

    // 5. Obsługa serwera WWW
    webProcess();

    // 6. Odświeżanie wyświetlacza (z ograniczoną częstotliwością)
    if ((now - lastDisplayRefresh) >= DISPLAY_REFRESH_MS) {
        oledUpdate();
        lastDisplayRefresh = now;
    }

    // --- PRZYGOTOWANIE POD RTC ---
    if (RTC_ENABLED) {
        // TODO: rtcProcess();
    }

    // --- PRZYGOTOWANIE POD PPS ---
    if (PPS_ENABLED) {
        // TODO: ppsProcess();
    }
}
