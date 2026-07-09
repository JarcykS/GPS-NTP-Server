/*
 * ======================================================================
 *  Projekt    : GPS NTP Server
 *  Plik       : NTP_Server.ino
 *  Opis       : Program główny
 *
 *  Wersja     : 04.00.01
 *  Środowisko : Arduino IDE 1.8.x
 *  Platforma  : ESP32 WROOM Mini D1
 *
 *  Autor      : JARCYK.NET
 * ======================================================================
 */

#include <Wire.h>
#include <WiFi.h>
#include <SSD1306.h>
#include <TinyGPSPlus.h>

#include "config.h"
#include "version.h"
#include "debug.h"

// ======================================================================
// Obiekty globalne
// ======================================================================

// OLED SSD1306
SSD1306 display(OLED_ADDRESS, PIN_OLED_SDA, PIN_OLED_SCL, GEOMETRY_64_48);

// GPS
TinyGPSPlus gps;
HardwareSerial neogps(1);

// ======================================================================
// Zmienne globalne
// ======================================================================

char buffer[32];

// ======================================================================
// SETUP
// ======================================================================

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  neogps.begin(GPS_BAUDRATE, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);

  display.init();
  display.flipScreenVertically();

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawString(0, 0, FW_NAME);
  display.drawString(0, 12, FW_FULL_VER);
  display.drawString(0, 24, FW_BUILD_DATE);
  display.drawString(0, 36, FW_BUILD_TIME);

  display.display();

  DEBUG_PRINTLN("====================================");
  DEBUG_PRINTLN(FW_NAME);
  DEBUG_PRINT("Wersja : ");
  DEBUG_PRINTLN(FW_FULL_VER);
  DEBUG_PRINT("Data   : ");
  DEBUG_PRINTLN(FW_BUILD_DATE);
  DEBUG_PRINT("Czas   : ");
  DEBUG_PRINTLN(FW_BUILD_TIME);
  DEBUG_PRINTLN("====================================");
}

// ======================================================================
// LOOP
// ======================================================================

void loop()
{
  // Kod programu będzie dodawany w kolejnych kompilacjach.
}