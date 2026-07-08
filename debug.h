/*
 * ======================================================================
 *  Projekt    : GPS NTP Server
 *  Plik       : debug.h
 *  Opis       : Ustawienia komunikatów diagnostycznych
 *
 *  Wersja     : 04.00.01
 *  Środowisko : Arduino IDE 1.8.x
 *  Platforma  : ESP32 WROOM Mini D1
 *
 *  Autor      : JARCYK.NET
 * ======================================================================
 */

#ifndef DEBUG_H
#define DEBUG_H

// ======================================================================
// Włączenie/Wyłączenie komunikatów diagnostycznych
// ======================================================================

#define DEBUG_SERIAL      1
#define DEBUG_GPS         1
#define DEBUG_WIFI        1
#define DEBUG_NTP         1
#define DEBUG_WEB         1
#define DEBUG_OLED        0

// ======================================================================
// Makra diagnostyczne
// ======================================================================

#if DEBUG_SERIAL

  #define DEBUG_PRINT(x)        Serial.print(x)
  #define DEBUG_PRINTLN(x)      Serial.println(x)
  #define DEBUG_PRINTF(...)     Serial.printf(__VA_ARGS__)

#else

  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)

#endif

#endif