/*
 * ntp.cpp
 * Implementacja serwera NTP zgodnego z RFC 5905 (NTPv4)
 *
 * Pakiet NTPv4 ma 48 bajtów. Kluczowe pola odpowiedzi:
 *  [0]     LI(2b) | VN(3b) | Mode(3b)
 *  [1]     Stratum
 *  [2]     Poll
 *  [3]     Precision (signed log2 seconds)
 *  [4-7]   Root Delay
 *  [8-11]  Root Dispersion
 *  [12-15] Reference ID (dla Stratum 1: 4 znaki ASCII, np. "GPS\0")
 *  [16-23] Reference Timestamp
 *  [24-31] Origin Timestamp
 *  [32-39] Receive Timestamp
 *  [40-47] Transmit Timestamp
 *          wersja 04.01
 */

#include "ntp.h"
#include "gps.h"
#include "config.h"

#include <WiFiUdp.h>
#include <sys/time.h>

static WiFiUDP udp;

#define NTP_PACKET_SIZE 48

// --- KONWERSJE POMOCNICZE ---

// Zapis 32-bitowej wartości big-endian
static void writeUint32(uint8_t* buf, uint8_t offset, uint32_t value) {
    buf[offset + 0] = (value >> 24) & 0xFF;
    buf[offset + 1] = (value >> 16) & 0xFF;
    buf[offset + 2] = (value >>  8) & 0xFF;
    buf[offset + 3] = (value      ) & 0xFF;
}

// Zapis znacznika czasu NTP (sekundy + ułamek 32-bit)
static void writeTimestamp(uint8_t* buf, uint8_t offset, const struct timeval* tv) {
    uint32_t ntpSec  = (uint32_t)tv->tv_sec + NTP_EPOCH_OFFSET;
    // (usec * 2^32) / 10^6 = usec * 4294.967296
    uint32_t ntpFrac = (uint32_t)((double)tv->tv_usec * 4294.967296);
    writeUint32(buf, offset,     ntpSec);
    writeUint32(buf, offset + 4, ntpFrac);
}

// --- INICJALIZACJA ---
void ntpInit(void) {
    udp.begin(NTP_PORT);
}

// --- OBSŁUGA ŻĄDAŃ NTP ---
void ntpProcess(void) {
    int packetSize = udp.parsePacket();
    if (packetSize != NTP_PACKET_SIZE) {
        return;
    }

    uint8_t packet[NTP_PACKET_SIZE];
    int bytesRead = udp.read(packet, NTP_PACKET_SIZE);
    if (bytesRead != NTP_PACKET_SIZE) {
        gpsData.ntpErrors++;
        return;
    }

    // Jeśli czas nie jest zsynchronizowany - nie odpowiadamy
    if (!gpsData.synced) {
        gpsData.ntpErrors++;
        return;
    }

    // Pobranie aktualnego czasu systemowego (ustawionego z GPS)
    struct timeval now;
    gettimeofday(&now, NULL);

    // --- BUDOWANIE ODPOWIEDZI NTPv4 ---

    // Pole 0: LI=0, VN=4 (NTPv4), Mode=4 (server) -> 0x24
    packet[0] = (0 << 6) | (NTP_VERSION << 3) | 4;

    // Pole 1: Stratum 1 (bezpośrednie źródło referencyjne)
    packet[1] = NTP_STRATUM;

    // Pole 2: Poll interval (log2 sekund)
    packet[2] = 6;

    // Pole 3: Precision (-20 = ~1μs)
    packet[3] = (uint8_t)(int8_t)(-20);

    // Pola 4-7: Root Delay = 0 (jesteśmy źródłem)
    writeUint32(packet, 4, 0);

    // Pola 8-11: Root Dispersion = 0
    writeUint32(packet, 8, 0);

    // Pola 12-15: Reference ID - dla Stratum 1 to 4-znakowy kod ASCII
    packet[12] = 'G';
    packet[13] = 'P';
    packet[14] = 'S';
    packet[15] = '\0';

    // Pola 16-23: Reference Timestamp (czas ostatniej synchronizacji)
    struct timeval refTv;
    refTv.tv_sec  = gpsData.lastSyncUnix;
    refTv.tv_usec = 0;
    writeTimestamp(packet, 16, &refTv);

    // Pola 24-31: Origin Timestamp - kopiowane z żądania (zostawiamy)

    // Pola 32-39: Receive Timestamp
    writeTimestamp(packet, 32, &now);

    // Pola 40-47: Transmit Timestamp
    writeTimestamp(packet, 40, &now);

    // --- PRZYGOTOWANIE POD PPS ---
    // W przyszłych wersjach: korekta Transmit Timestamp
    // o gpsData.ppsOffsetUs dla dokładności mikrosekundowej
    if (PPS_ENABLED && gpsData.ppsAvailable) {
        // TODO: modyfikacja pakietu na podstawie PPS
    }

    // Wysyłka odpowiedzi
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(packet, NTP_PACKET_SIZE);
    udp.endPacket();

    gpsData.ntpRequests++;
}
