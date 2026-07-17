/*
 * oled.h
 * Moduł wyświetlacza OLED SSD1306 64x48
 * wersja 04.01
 */

#ifndef OLED_H
#define OLED_H

// Inicjalizacja wyświetlacza
void oledInit(void);

// Aktualizacja zawartości wyświetlacza (odczytuje gpsData)
void oledUpdate(void);

#endif // OLED_H
