# GPS-NTP-Server
Small Server NTP based on ESP32 and GPS receiver

Celem projektu jest stworzenie małego i taniego serwera czasu NTP opartego na module ESP32 i GPS który może pracować w sieciach zamkniętych bez dostępu do interentu, sieciach domowych gdzie jest wymagana synchronizacja czasu wszystkich urządzeń.
-----------------------------------------------------------------------------------------------------------------------------------------------
The aim of the project is to create a small and cheap NTP time server based on the ESP32 and GPS module that can work in closed networks without access to the Internet, home networks where time synchronization of all devices is required.

-------------------------------------------------------
*                   Hardware                          * 
-------------------------------------------------------

1. ESP32 WROOM Mini D1        <img width="600" height="600" alt="obraz" src="https://github.com/user-attachments/assets/e08aaac7-1a63-4b05-a877-0473ba67b984" />


2. display: OLED SSD1306 64×48, 0.66"    <img width="600" height="600" alt="obraz" src="https://github.com/user-attachments/assets/6b34e40e-f6ef-4ad7-9dc0-a5ba771a526e" />


3. GPS modul GY-NEO6MV2      <img width="600" height="600" alt="obraz" src="https://github.com/user-attachments/assets/08ebf989-ee2d-4bbc-8443-0fa88ef10a84" />


4. Tactile Switch 6 x 6 mm  <img width="150" height="136" alt="obraz" src="https://github.com/user-attachments/assets/7e350293-9bb6-4818-9de6-a397a484ab22" />


5. dc-dc converter 7-26V to 5V  <img width="600" height="600" alt="obraz" src="https://github.com/user-attachments/assets/ebf86846-ba62-4bb7-9a92-927c17304ef4" />


6. Modular housing for DIN rail (Kradex)  <img width="956" height="900" alt="obraz" src="https://github.com/user-attachments/assets/31909598-249b-471e-a2f0-7a15cf895a16" />


------------------------------------------------------
*              Project architecture:                *
------------------------------------------------------

  GPS      
   │        
TinyGPS++  
   │        
gps.cpp    
   │      
gps.h      
   │      
   ├── oled.cpp  
   ├── ntp.cpp  
   ├── web.cpp  
   └── NTP_Server_04xx.ino  
  

------------------------------------------------------
*               Functionality                        *
------------------------------------------------------
✅ Arduino IDE 1.8.x

✅ ESP32 WROOM Mini D1

✅ OLED SSD1306 

✅ TinyGPS++

✅ WiFiManager

✅ classic HTML (page += ...)

✅ moduls .cpp/.h

✅ comments in Polish

✅ one global GPS_DATA structure

✅ ready for RTC modul

✅ ready for PPS (in the future)

✅ Stratum 1 compliant with RFC 5905

------------------------------------------------------

Checking if the NTP server is working:
- for WINDOWS Powershell enter the command:  "w32tm /stripchart /computer:192.168.x.x /samples:5 /dataonly"
- for Linux/macOS bash: "sudo ntpdate -q 192.168.x.x" 

here is an example answer:  
"Tracking 192.168.1.102 [192.168.1.102:123].  
Collecting 5 samples.  
The current time is 16.07.2026 01:34:09.  
01:34:09, +1996573624.8063002s  
01:34:11, +1996573625.8819847s  
01:34:14, +1996573627.0076658s  
01:34:16, +1996573628.1843752s  
01:34:18, +1996573629.2817115s  
"

------------------------------------------------------
Finally, you need to set a new NTP server address in your Router.

------------------------------------------------------

Jarcyk.net
