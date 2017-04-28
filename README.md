# ESP8266-Lightswitch
Firmware for ESP8266 to control a lightswitch via Microservo SG90

## How to use
1. Open NodeMCU_1.ino with the Arduino IDE
2. Insert WiFi credentials
3. Insert MQTT credentials
4. Install Librarys (see in the Sketch)
5. Connect SG90 Microservo to your Arduino Pins `Ground - VV - D3`
6. Upload and flash the firmware to NodeMCU board
7. Configure your MQTT Controller Hub (Mosquitto in my case)
8. Have fun
