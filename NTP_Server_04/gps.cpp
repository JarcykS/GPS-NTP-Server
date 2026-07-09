/*
 * ======================================================================
 *  Projekt    : GPS NTP Server
 *  Plik       : gps.cpp
 *  Opis       : Obsługa odbiornika GPS
 *
 *  Wersja     : 04.00.02
 * ======================================================================
 */

#include <Arduino.h>

#include "gps.h"
#include "debug.h"

// ----------------------------------------------------------------------
// Obiekty
// ----------------------------------------------------------------------

TinyGPSPlus gps;
HardwareSerial neogps(1);

// ----------------------------------------------------------------------

void gpsInit(void)
{
    neogps.begin(GPS_BAUDRATE, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);

    DEBUG_PRINTLN("GPS Init OK");
}

// ----------------------------------------------------------------------

void gpsRead(void)
{
    while (neogps.available())
    {
        gps.encode(neogps.read());
    }
}

// ----------------------------------------------------------------------

bool gpsValid(void)
{
    return gps.location.isValid();
}