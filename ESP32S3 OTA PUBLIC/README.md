# ESP32 OTA by Public Server (VPS)
This folder contains ESP32 Over-The-Air (OTA) Program in C++ over GitHub Public Server.

## Scheme
- Turn on the ESP32
- ESP32 will connect to your WiFi router
- ESP32 will access the Public Server to get firmware.json
- ESP32 will check, there is a firmware update or not
- Add `firmware_x.bin` file to Public Server and edit the `firmware.json`
- ESP32 automatically updates the firmware

## What did I do?
I just added these codes to my main.cpp program.
```
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>

#define FIRMWARE_VERSION 1.0
#define UPDATE_URL "https://octarudin.github.io/ESP32-OTA-Update/firmware.json"
```  
and make a routine task with RTOS to check the firmware update.

## What if the OTA Process failed?
It will roll back to older firmware.
