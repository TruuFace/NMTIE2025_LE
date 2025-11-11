# GAIA Makerspace - Ultrasonic Distance Sensor Demo

This project was created by **Austin Bryant** for the **NMTIE Conference Workshop**.  
It showcases how to combine sensors, LEDs, and Wi-Fi connectivity using the **Arduino UNO R4 WiFi**, and includes a **Tinkercad simulation** for the UNO R3.

---

##  Hardware Setup

| Component | Pin | Description |
|------------|-----|-------------|
| HC-SR04 Trig | A0 | Trigger output |
| HC-SR04 Echo | A1 | Echo input |
| LED1 | D10 | Red – farthest |
| LED2 | D11 | Orange |
| LED3 | D12 | Purple |
| LED4 | D13 | Blue – closest |
| Buzzer | D5 | Audio feedback |

---

## Code Versions

### UNO R4 WiFi Version  
Includes built-in **Access Point** and a live web dashboard.  
 [View Code](Arduino_Code/UNO_R4_WiFi_Distance_Sensor.ino)

When uploaded, the board hosts its own Wi-Fi network:
SSID: Arduino-Distance-Sensor
Password: demo1234
Dashboard: http://192.168.4.1

yaml
Copy code

---

### UNO R3 (Tinkercad Version)  
Simulation-ready and identical LED/buzzer logic.  
[Open in Tinkercad](https://www.tinkercad.com/things/h186rsrGwWR-nmtie-2025-example) 
[View Code](Arduino_Code/UNO_R3_Distance_Sensor.ino)

---

##  Project Overview

This project uses an ultrasonic sensor to detect distance and visualizes proximity using LEDs and sound.
- The **farther** the object → fewer LEDs lit.  
- The **closer** the object → more LEDs + buzzer tone.  
- The **Wi-Fi version** hosts a real-time HTML dashboard.  

---

##  Learning Outcomes
- Understand ultrasonic distance measurement.  
- Map sensor data to real-world outputs.  
- Learn how to host a local web dashboard with Arduino Wi-Fi boards.  
- Simulate and prototype circuits virtually in **Tinkercad**.  

---

##  Credits
**Created by:** Austin Bryant  
**Organization:** GAIA Makerspace, UTEP Learning Environments  
**Conference:** NMTIE 2025  
