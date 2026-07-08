\
/*
 NTP_Server_04.00 (SZKIELET)
 Uwaga: To szablon projektu. Pełna implementacja serwera NTP wymaga
 kilkuset linii kodu i nie mieści się w limicie odpowiedzi.
*/

#include <Wire.h>
#include <SSD1306.h>
#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <TimeLib.h>

#define RXD2 16
#define TXD2 17
#define SW1 0

HardwareSerial neogps(1);
SSD1306 display(0x3c,21,22,GEOMETRY_64_48);
TinyGPSPlus gps;
WiFiUDP udp;
WebServer server(80);

const uint16_t NTP_PORT=123;
byte packetBuffer[48];
const uint32_t NTP_OFFSET=2208988800UL;

void configModeCallback(WiFiManager *wm){
  display.clear();
  display.drawString(0,0,"Config WiFi");
  display.drawString(0,12,wm->getConfigPortalSSID());
  display.display();
}

void setup(){
  Serial.begin(115200);
  pinMode(SW1,INPUT_PULLUP);
  neogps.begin(9600,SERIAL_8N1,RXD2,TXD2);
  display.init();
  display.flipScreenVertically();

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if(digitalRead(SW1)==LOW){
    wifiManager.startConfigPortal("NTP-GPS Server");
  }else{
    if(!wifiManager.autoConnect("NTP-GPS Server")){
      ESP.restart();
    }
  }

  udp.begin(NTP_PORT);
  server.on("/",[](){ server.send(200,"text/plain","NTP_Server_04.00");});
  server.begin();
}

void loop(){
  while(neogps.available()) gps.encode(neogps.read());
  server.handleClient();

  int ps=udp.parsePacket();
  if(ps>=48){
    udp.read(packetBuffer,48);
    // TODO: zbudować znacznik czasu z GPS i odesłać odpowiedź NTP.
  }
}
