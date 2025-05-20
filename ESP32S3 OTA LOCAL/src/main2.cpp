#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN    48
#define NUMPIXELS       1

const char* ssid = "Aprian";
const char* password = "telpwifi";

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

TaskHandle_t justPrintTask;

void showGreenLED(bool status);
void blinkGreenLED(void *pvParam);
void justPrint(void *pvParam);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  pixels.begin();
  pixels.setBrightness(50); // nilai 0-255, bisa disesuaikan
  pixels.show(); // update LED

  // Inisialisasi OTA
  ArduinoOTA.setHostname("esp32-ota-2");
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start OTA update");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");

  xTaskCreatePinnedToCore(blinkGreenLED, "JustPrint", 2048, NULL, 1, &justPrintTask, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
}

void justPrint(void *pvParam) {
  while(true) {
    Serial.println("This is ESP32-2");
    Serial.println("IP Address: " + WiFi.localIP().toString() );
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void blinkGreenLED(void *pvParam) {
  while (true) {  // Loop tak terbatas
    showGreenLED(true);
    vTaskDelay(pdMS_TO_TICKS(200));
    showGreenLED(false);
    vTaskDelay(pdMS_TO_TICKS(800));
  }
}

void showGreenLED(bool status) {
  if (status) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Matikan LED
  }
  pixels.show();
}