/*
 * config.h
 * Stałe konfiguracyjne projektu ESP32 GPS NTP Server v04.01
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- WERSJA FIRMWARE ---
#define FW_VERSION      "04.03"
#define FW_NAME         "GPS-NTP-Server"

// --- PINY SPRZĘTOWE ---
#define GPS_RX_PIN      16
#define GPS_TX_PIN      17
#define GPS_BAUD        9600

#define OLED_SDA        21
#define OLED_SCL        22
#define OLED_ADDR       0x3C
#define OLED_WIDTH      64
#define OLED_HEIGHT     48

#define BUTTON_PIN      2

// --- PRZYGOTOWANIE POD PPS (nieaktywne w v04.01) ---
#define PPS_PIN         0
#define PPS_ENABLED     0

// --- PRZYGOTOWANIE POD RTC (nieaktywne w v04.01) ---
#define RTC_ENABLED     0

// --- SERWER NTP ---
#define NTP_PORT        123
#define NTP_VERSION     4       // NTPv4 (RFC 5905)
#define NTP_STRATUM     1       // Stratum 1 - źródło bezpośrednie
#define NTP_EPOCH_OFFSET 2208988800UL  // 1900 -> 1970 w sekundach

// --- SERWER WWW ---
#define WEB_PORT        80

// --- INTERWAŁY [ms] ---
#define GPS_SYNC_INTERVAL_MS   1000
#define DISPLAY_REFRESH_MS     500
#define BUTTON_DEBOUNCE_MS     200

#endif // CONFIG_H
