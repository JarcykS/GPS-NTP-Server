/*
 * web.cpp
 * Implementacja serwera WWW
 * Budowanie HTML przez klasyczną konkatenację: page += "..."
 * wersja 04.01
 */

#include "web.h"
#include "gps.h"
#include "config.h"

#include <WebServer.h>
#include <WiFi.h>
#include <String.h>
#include <stdio.h>

static WebServer httpServer(WEB_PORT);

// --- GENERATOR STRONY HTML ---
static String buildStatusPage(void) {
    String page;
    page.reserve(1500);

    page += "<!DOCTYPE html><html><head>";
    page += "<meta charset='UTF-8'>";
    page += "<title>";
    page += FW_NAME;
    page += "</title>";
    page += "<style>";
    page += "body{font-family:sans-serif;background:#111;color:#eee;margin:20px}";
    page += "h1{color:#4af}table{border-collapse:collapse;width:100%;max-width:500px}";
    page += "td,th{border:1px solid #444;padding:6px 10px;text-align:left}";
    page += "th{background:#222}.ok{color:#4f4}.wait{color:#fa0}";
    page += "</style></head><body>";

    page += "<h1>";
    page += FW_NAME;
    page += " v";
    page += FW_VERSION;
    page += "</h1>";

    // ---- Sekcja GPS ----
    page += "<h2>Dane z GPS</h2><table>";
    page += "<tr><th>Parametr</th><th>Wartość</th></tr>";

    page += "<tr><td>Status</td><td>";
    page += gpsData.valid ? "<span class='ok'>VALID</span>"
                          : "<span class='wait'>NO FIX</span>";
    page += "</td></tr>";

    page += "<tr><td>Satelity</td><td>";
    page += String(gpsData.satellites);
    page += "</td></tr>";

    page += "<tr><td>Czas UTC</td><td>";
    if (gpsData.timeValid) {
        char buf[28];
        snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u.%02u",
                 gpsData.year, gpsData.month, gpsData.day,
                 gpsData.hour, gpsData.minute, gpsData.second,
                 gpsData.centisecond);
        page += buf;
    } else {
        page += "--";
    }
    page += "</td></tr>";

    page += "<tr><td>Pozycja</td><td>";
    if (gpsData.locationValid) {
        char buf[40];
        snprintf(buf, sizeof(buf), "%.6f, %.6f",
                 gpsData.latitude, gpsData.longitude);
        page += buf;
    } else {
        page += "--";
    }
    page += "</td></tr></table>";

    // ---- Sekcja NTP ----
    page += "<h2>Serwer NTP</h2><table>";
    page += "<tr><th>Parametr</th><th>Wartość</th></tr>";

    page += "<tr><td>Port</td><td>";
    page += String(NTP_PORT);
    page += "</td></tr>";

    page += "<tr><td>Wersja</td><td>NTPv";
    page += String(NTP_VERSION);
    page += " (RFC 5905)</td></tr>";

    page += "<tr><td>Stratum</td><td>";
    page += String(NTP_STRATUM);
    page += "</td></tr>";

    page += "<tr><td>Reference ID</td><td>GPS</td></tr>";

    page += "<tr><td>Synchronizacja</td><td>";
    page += gpsData.synced ? "<span class='ok'>TAK</span>"
                           : "<span class='wait'>CZEKA</span>";
    page += "</td></tr>";

    page += "<tr><td>Żądania</td><td>";
    page += String((unsigned long)gpsData.ntpRequests);
    page += "</td></tr>";

    page += "<tr><td>Błędy</td><td>";
    page += String((unsigned long)gpsData.ntpErrors);
    page += "</td></tr></table>";

    // ---- Sekcja System ----
    page += "<h2>System</h2><table>";
    page += "<tr><th>Parametr</th><th>Wartość</th></tr>";

    page += "<tr><td>IP</td><td>";
    page += WiFi.localIP().toString();
    page += "</td></tr>";

    page += "<tr><td>RSSI</td><td>";
    page += String(WiFi.RSSI());
    page += " dBm</td></tr>";

    page += "<tr><td>Uptime</td><td>";
    page += String((unsigned long)(millis() / 1000));
    page += " s</td></tr>";

    page += "<tr><td>RTC</td><td>";
    page += RTC_ENABLED ? "tak" : "nie (zarezerwowane)";
    page += "</td></tr>";

    page += "<tr><td>PPS</td><td>";
    page += PPS_ENABLED ? "tak" : "nie (zarezerwowane)";
    page += "</td></tr></table>";

    // ---- Sekcja STOPKA ----
    page += "<h2> - </h2><table>";
    page += "<tr><th>Made by</th><th>JARCYK.NET</th></tr>";

    page += "<tr><td>Autor</td><td>";
    page += "<p>Jaroslaw Swirski<p/>";
    page += "</td></tr>";

    page += "<tr><td>Data</td><td>";
    page += "<p>2026<p/>";
    page += "</td></tr>";

    page += "<tr><td>link:</td><td>";
    page += "<a href=>https://jarcyk.net/arduino/arduino.htm<a/>";
    page += "</td></tr></table>";
    
    page += "</body></html>";
    return page;
}

// --- HANDLER STRONY GŁÓWNEJ ---
static void handleRoot(void) {
    String page = buildStatusPage();
    httpServer.send(200, "text/html", page);
}

// --- INICJALIZACJA ---
void webInit(void) {
    httpServer.on("/", handleRoot);
    httpServer.begin();
}

// --- OBSŁUGA ŻĄDAŃ ---
void webProcess(void) {
    httpServer.handleClient();
}
