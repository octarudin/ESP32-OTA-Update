#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32httpUpdate.h>
#include <Adafruit_NeoPixel.h>

#define FIRMWARE_VERSION 1.1
#define UPDATE_URL "https://octarudin.github.io/ESP32-OTA-Update/firmware.json"

#define NEOPIXEL_PIN    48
#define NUMPIXELS       1

TaskHandle_t task0;
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
const char* ssid = "Aprian";
const char* password = "telpwifi";

void ledBlink(void *pvParam) {
  while (1)
  {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(200));
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(800));
  }
  
}

t_httpUpdate_return updateFirmware(String url_update) {
  t_httpUpdate_return ret;
  if (WiFi.status() == WL_CONNECTED) {
    ret = ESPhttpUpdate.update(url_update);
    Serial.begin(115200);
    // Serial print will not be executed
    switch (ret)
    {
    case HTTP_UPDATE_OK:
      Serial.println("Update succeed.");
      return ret;
      break;
    case HTTP_UPDATE_FAILED:
      Serial.println("Update failed.");
      return ret;
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No Update.");
      return ret;
      break;
    }
  }
}

void checkUpdate() {
  Serial.println("Checking update");
  HTTPClient http;
  String response;
  String url = UPDATE_URL;
  http.begin(url);
  http.GET();

  response = http.getString();

  Serial.println(response);

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, response);
  JsonObject obj = doc.as<JsonObject>();

  String version = obj[String("version")];
  String url_update = obj[String("url")];

  Serial.println(version);
  Serial.println(url_update);

  if (version.toDouble() > FIRMWARE_VERSION) {
    Serial.println("Update available");
    if (updateFirmware(url_update) == HTTP_UPDATE_OK) {
      // Serial.println will not be executed
      Serial.println("Restarting ESP32");
      Serial.println();
      ESP.restart();
    }
  } else {
    Serial.println("No Update available");
  }
  Serial.println();
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("WIFI init");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pixels.begin();
  pixels.setBrightness(50); // nilai 0-255, bisa disesuaikan
  pixels.show(); // update LED

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

  xTaskCreatePinnedToCore(ledBlink, "checkUpdate", 2048, NULL, 1, &task0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkUpdate();
  delay(5000);
}

