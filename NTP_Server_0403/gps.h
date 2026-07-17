/*
 * gps.h
 * Centralny moduł GPS - definicja struktury GPS_DATA
 * oraz funkcji odczytu z modułu GNSS.
 *
 * Struktura GPS_DATA jest JEDYNĄ globalną strukturą danych projektu.
 * Wszystkie pozostałe moduły (oled, ntp, web) odczytują dane
 * bezpośrednio z tej struktury.
 * wersja 04.01
 */

#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include <stdbool.h>

// --- TRYBY WYŚWIETLANIA (współdzielone z modułem oled) ---
enum DisplayMode {
    MODE_GPS_INFO = 0,    // współrzędne, satelity
    MODE_TIME_UTC,        // czas UTC
    MODE_NTP_STATUS,      // status serwera NTP
    MODE_SYSTEM_INFO,     // IP, wersja FW
    MODE_COUNT            // liczba trybów
};

// --- GŁÓWNA STRUKTURA DANYCH PROJEKTU ---
// Jedna instancja tej struktury (gpsData) zawiera wszystkie dane
// dzielone między moduły: GPS, NTP, WWW, OLED, RTC, PPS.
typedef struct {
    // ---- Dane z GPS (TinyGPS++) ----
    bool     valid;            // ogólna poprawność danych
    bool     timeValid;        // czas jest poprawny
    bool     dateValid;        // data jest poprawna
    bool     locationValid;    // pozycja jest poprawna

    uint16_t year;             // rok UTC
    uint8_t  month;            // miesiąc 1-12
    uint8_t  day;              // dzień 1-31
    uint8_t  hour;             // godzina 0-23 UTC
    uint8_t  minute;           // minuta 0-59
    uint8_t  second;           // sekunda 0-59
    uint16_t centisecond;      // setne sekundy (0-99)

    double   latitude;
    double   longitude;
    double   altitude;         // [m]
    uint8_t  satellites;

    // ---- Status synchronizacji czasu ----
    bool     synced;           // czy czas ustawiony z GPS
    uint32_t lastSyncMillis;   // millis() ostatniej synchronizacji
    uint32_t lastSyncUnix;     // timestamp Unix ostatniej synchronizacji

    // ---- Statystyki NTP ----
    uint32_t ntpRequests;
    uint32_t ntpErrors;

    // ---- PRZYSZŁA OBSŁUGA RTC ----
    bool     rtcAvailable;
    bool     rtcSynced;
    uint32_t rtcLastRead;

    // ---- PRZYGOTOWANIE POD PPS ----
    bool     ppsAvailable;
    uint32_t ppsLastEdgeMs;
    uint32_t ppsLastEdgeUs;
    uint32_t ppsCount;
    int32_t  ppsOffsetUs;

    // ---- Stan interfejsu ----
    uint8_t  displayMode;
    bool     buttonPressed;
} GPS_DATA;

// --- GLOBALNA ZMIENNA PROJEKTU ---
// Definicja znajduje się w gps.cpp
extern GPS_DATA gpsData;

// --- FUNKCJE MODUŁU GPS ---

// Inicjalizacja struktury i portu UART GPS
void gpsInit(void);

// Odczyt i dekodowanie danych z UART
// Zwraca true gdy otrzymano nową, poprawną ramkę
bool gpsProcess(void);

// Sprawdzenie czy należy wykonać synchronizację zegara
bool gpsShouldSync(void);

// Synchronizacja zegara systemowego ESP32 z danymi GPS
// Zwraca true gdy ustawienie się powiodło
bool gpsSyncSystemTime(void);

// Konwersja czasu GPS na timestamp Unix
uint32_t gpsToUnixTimestamp(void);

// Formatowanie czasu UTC do bufora tekstowego
void gpsFormatTimeUTC(char* buf, uint8_t bufSize);

#endif // GPS_H
