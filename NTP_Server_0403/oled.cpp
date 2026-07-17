/*
 * oled.cpp
 * Implementacja modułu wyświetlacza OLED
 * ODWRÓCONY o 180° - ręczne przesunięcie współrzędnych Y
 * wersja 04.03
 */

#include "oled.h"
#include "gps.h"
#include "config.h"

#include <SSD1306.h>
#include <WiFi.h>
#include <stdio.h>

static SSD1306 oled(OLED_ADDR, OLED_SDA, OLED_SCL);

void oledInit(void) {
    oled.init();
    oled.setFont(ArialMT_Plain_10);
    oled.setTextAlignment(TEXT_ALIGN_LEFT);
    
    // BEZ flipScreenVertically() - odwracamy ręcznie w kodzie rysującym
    
    oled.clear();
    oled.drawString(36, 38, FW_NAME);      // Zamiast 0
    oled.drawString(36, 22, "v" FW_VERSION); // Zamiast 16
    oled.display();
}

// --- TRYB 0: INFORMACJE GPS (ODWRÓCONY) ---
static void drawGpsInfo(void) {
    char buf[16];

    oled.drawString(36, 38, "GPS");  // Zamiast 0

    if (gpsData.locationValid) {
        snprintf(buf, sizeof(buf), "%.4f", gpsData.latitude);
        oled.drawString(36, 26, buf);  // Zamiast 12
        snprintf(buf, sizeof(buf), "%.4f", gpsData.longitude);
        oled.drawString(36, 14, buf);  // Zamiast 24
    } else {
        oled.drawString(36, 26, "No fix");
    }

    snprintf(buf, sizeof(buf), "SAT:%u", gpsData.satellites);
    oled.drawString(36, 0, buf);  // Zamiast 36
}

// --- TRYB 1: CZAS UTC (ODWRÓCONY) ---
static void drawTimeUtc(void) {
    char buf[16];

    oled.drawString(36, 38, "UTC");  // Zamiast 0

    if (gpsData.timeValid) {
        snprintf(buf, sizeof(buf), "%02u:%02u:%02u",
                 gpsData.hour, gpsData.minute, gpsData.second);
        oled.drawString(36, 24, buf);  // Zamiast 14

        snprintf(buf, sizeof(buf), "%02u.%02u.%02u",
                 gpsData.day, gpsData.month, gpsData.year % 100);
        oled.drawString(36, 12, buf);  // Zamiast 26
    } else {
        oled.drawString(36, 24, "--:--:--");
    }

    oled.drawString(36, 0, gpsData.synced ? "SYNC" : "WAIT");  // Zamiast 38
}

// --- TRYB 2: STATUS NTP (ODWRÓCONY) ---
static void drawNtpStatus(void) {
    char buf[16];

    oled.drawString(36, 38, "NTP");  // Zamiast 0
    
    snprintf(buf, sizeof(buf), "S:%u R:%lu",
             NTP_STRATUM, (unsigned long)gpsData.ntpRequests);
    oled.drawString(30, 26, buf);  // Zamiast 12
    
    oled.drawString(36, 14, gpsData.synced ? "OK" : "WAIT");  // Zamiast 24
    
    snprintf(buf, sizeof(buf), "E:%lu", (unsigned long)gpsData.ntpErrors);
    oled.drawString(36, 0, buf);  // Zamiast 36
}

// --- TRYB 3: INFORMACJE SYSTEMOWE (ODWRÓCONY) ---
static void drawSystemInfo(void) {
    oled.drawString(36, 38, "SYS");  // Zamiast 0
    oled.drawString(36, 26, WiFi.localIP().toString());  // Zamiast 12
    oled.drawString(16, 14, "v" FW_VERSION);  // Zamiast 24
    oled.drawString(16, 0, "PPS:--");  // Zamiast 36
}

void oledUpdate(void) {
    oled.clear();

    switch (gpsData.displayMode) {
        case MODE_GPS_INFO:    drawGpsInfo();    break;
        case MODE_TIME_UTC:    drawTimeUtc();    break;
        case MODE_NTP_STATUS:  drawNtpStatus();  break;
        case MODE_SYSTEM_INFO: drawSystemInfo(); break;
        default:               drawGpsInfo();    break;
    }

    oled.display();
}
