/*
 * ntp.h
 * Serwer NTP zgodny z RFC 5905, Stratum 1
 * wersja 04.01
 */

#ifndef NTP_H
#define NTP_H

// Inicjalizacja serwera NTP (nasłuch UDP 123)
void ntpInit(void);

// Obsługa przychodzących żądań NTP - wywoływać w loop()
void ntpProcess(void);

#endif // NTP_H
