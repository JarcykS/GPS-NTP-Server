/*
 * web.h
 * Serwer WWW ze stroną statusu (klasyczne HTML)
 * wersja 04.01
 */

#ifndef WEB_H
#define WEB_H

// Inicjalizacja serwera WWW
void webInit(void);

// Obsługa żądań HTTP - wywoływać w loop()
void webProcess(void);

#endif // WEB_H
