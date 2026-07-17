/*
 * gps.cpp
 * Implementacja modułu GPS - odczyt z TinyGPS++, synchronizacja czasu
 * wersja 04.02
 */

#include "gps.h"
#include "config.h"

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>

// --- DEFINICJA JEDYNEJ GLOBALNEJ STRUKTURY DANYCH ---
GPS_DATA gpsData;

// Obiekt biblioteki TinyGPS++ (biblioteka zewnętrzna)
static TinyGPSPlus tinyGps;

// --- INICJALIZACJA ---
void gpsInit(void) {
    // Wyzerowanie całej struktury
    memset(&gpsData, 0, sizeof(GPS_DATA));

    // Domyślne wartości logiczne
    gpsData.valid         = false;
    gpsData.timeValid     = false;
    gpsData.dateValid     = false;
    gpsData.locationValid = false;
    gpsData.synced        = false;

    // Stan modułów rozszerzonych (zarezerwowane na przyszłość)
    gpsData.rtcAvailable = (RTC_ENABLED != 0);
    gpsData.ppsAvailable = (PPS_ENABLED != 0);

    // Domyślny tryb wyświetlania
    gpsData.displayMode  = MODE_GPS_INFO;
    gpsData.buttonPressed = false;

    // Uruchomienie portu UART GPS
    Serial1.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

// --- PRZETWARZANIE DANYCH Z UART ---
bool gpsProcess(void) {
    bool newFix = false;

    // Odczyt wszystkich dostępnych bajtów z UART
    while (Serial1.available() > 0) {
        if (tinyGps.encode(Serial1.read())) {
            newFix = true;
        }
    }

// Aktualizacja pól struktury na podstawie TinyGPS++
// UWAGA: Usunięto warunek isUpdated() - wystarczy isValid()
// isUpdated() wymaga aby dane były "świeże" w bieżącej sesji,
// co może powodować fałszywe negatives gdy ramki NMEA przychodzą z różną częstotliwością

gpsData.timeValid     = tinyGps.time.isValid();
gpsData.dateValid     = tinyGps.date.isValid();
gpsData.locationValid = tinyGps.location.isValid();
gpsData.valid         = gpsData.timeValid && gpsData.dateValid;

if (gpsData.timeValid) {
    gpsData.year        = tinyGps.date.year();
    gpsData.month       = tinyGps.date.month();
    gpsData.day         = tinyGps.date.day();
    gpsData.hour        = tinyGps.time.hour();
    gpsData.minute      = tinyGps.time.minute();
    gpsData.second      = tinyGps.time.second();
    gpsData.centisecond = tinyGps.time.centisecond();
}

if (gpsData.locationValid) {
    gpsData.latitude  = tinyGps.location.lat();
    gpsData.longitude = tinyGps.location.lng();
    gpsData.altitude  = tinyGps.altitude.meters();
}

if (tinyGps.satellites.isValid()) {
    gpsData.satellites = tinyGps.satellites.value();
}

    // --- PRZYGOTOWANIE POD PPS ---
    // W przyszłych wersjach: korekta centisecond na podstawie PPS
    if (PPS_ENABLED) {
        // TODO: aktualizacja ppsAvailable, ppsLastEdgeMs, ppsOffsetUs
    }

    return newFix;
}

// --- KONWERSJA CZASU GPS NA UNIX TIMESTAMP ---
uint32_t gpsToUnixTimestamp(void) {
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));

    timeinfo.tm_year = gpsData.year - 1900;
    timeinfo.tm_mon  = gpsData.month - 1;
    timeinfo.tm_mday = gpsData.day;
    timeinfo.tm_hour = gpsData.hour;
    timeinfo.tm_min  = gpsData.minute;
    timeinfo.tm_sec  = gpsData.second;

    // Wymuszenie strefy czasowej UTC, aby mktime() traktował czas jako UTC
    setenv("TZ", "UTC0", 1);
    tzset();
    
    // Teraz mktime zwróci poprawny timestamp Unix dla czasu UTC
    time_t t = mktime(&timeinfo);

    return (uint32_t)t;
}

// --- SYNCHRONIZACJA ZEGARA SYSTEMOWEGO Z GPS ---
bool gpsSyncSystemTime(void) {
    if (!gpsData.timeValid || !gpsData.dateValid) {
        return false;
    }

    struct timeval tv;
    tv.tv_sec  = gpsToUnixTimestamp();
    tv.tv_usec = (uint32_t)gpsData.centisecond * 10000;  // cs -> us

    if (settimeofday(&tv, NULL) != 0) {
        return false;
    }

    // Aktualizacja pól statusu
    gpsData.synced       = true;
    gpsData.lastSyncUnix = (uint32_t)tv.tv_sec;
    gpsData.lastSyncMillis = millis();

    return true;
}

// --- CZY NALEŻY WYKONAĆ SYNCHRONIZACJĘ ---
bool gpsShouldSync(void) {
    if (!gpsData.valid) return false;

    // Pierwsza synchronizacja
    if (!gpsData.synced) return true;

    // Kolejne synchronizacje co GPS_SYNC_INTERVAL_MS
    if ((millis() - gpsData.lastSyncMillis) >= GPS_SYNC_INTERVAL_MS) {
        return true;
    }

    return false;
}

// --- FORMATOWANIE CZASU UTC ---
void gpsFormatTimeUTC(char* buf, uint8_t bufSize) {
    snprintf(buf, bufSize, "%04u-%02u-%02u %02u:%02d:%02u",
             gpsData.year, gpsData.month, gpsData.day,
             gpsData.hour, gpsData.minute, gpsData.second);
}
