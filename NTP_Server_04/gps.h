/*
 * ======================================================================
 *  Projekt    : GPS NTP Server
 *  Plik       : gps.h
 *  Opis       : Obsługa odbiornika GPS
 *
 *  Wersja     : 04.00.02
 * ======================================================================
 */

#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
#include "config.h"

// ----------------------------------------------------------------------
// Zmienne globalne
// ----------------------------------------------------------------------

extern TinyGPSPlus gps;
extern HardwareSerial neogps;

// ----------------------------------------------------------------------
// Funkcje
// ----------------------------------------------------------------------

void gpsInit(void);
void gpsRead(void);
bool gpsValid(void);

#endif