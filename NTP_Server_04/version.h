/*
 * ======================================================================
 *  Projekt    : GPS NTP Server
 *  Plik       : version.h
 *  Opis       : Informacje o wersji oprogramowania
 *
 *  Wersja     : 04.00.01
 *  Środowisko : Arduino IDE 1.8.x
 *  Platforma  : ESP32 WROOM Mini D1
 *
 *  Autor      : JARCYK.NET
 * ======================================================================
 */

#ifndef VERSION_H
#define VERSION_H

// ======================================================================
// Informacje o projekcie
// ======================================================================

#define FW_NAME        "GPS NTP Server"
#define FW_VERSION     "04.01"
#define FW_BUILD       "01"

#define FW_AUTHOR      "JARCYK.NET"

// Data i czas kompilacji
#define FW_BUILD_DATE  __DATE__
#define FW_BUILD_TIME  __TIME__

// Łańcuch wersji wyświetlany na OLED, WWW i UART
#define FW_FULL_VER    FW_VERSION "." FW_BUILD

#endif