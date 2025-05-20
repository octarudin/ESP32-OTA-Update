# ESP32S3 OTA LOCAL
This folder contains ESP32 Over-The-Air (OTA) Program in C++ over Local Wi-Fi.

## Scheme
- Just turn on the ESP32.
- ESP32 will connect to your house Wi-Fi Router.
- After it got an IP Address, you can rebuild and reupload the program to ESP32 over the network.

## What did I do?
I add these keywords into platform.ini.  
```
upload_protocol = espota        ; Upload by network  
upload_port     = 192.168.0.18  ; ESP IP Address
```  
  
and  
  
Use this library to my program.  
`#include <ArduinoOTA.h>`


## What if the OTA process error / not finished?
The updated / latest program will not be executed by ESP32. The older version will be executed.
