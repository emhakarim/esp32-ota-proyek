#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

// ----- Wi-Fi -----
const char* ssid = "YAE_PRODUCTION";
const char* password = "1+5+7=13";

// ----- Versi dan URL -----
#define FIRMWARE_VERSION 2  // ⭐ UBAH INI MENJADI 2
String version_url = "https://raw.githubusercontent.com/emhakarim/esp32-ota-proyek/main/version.txt";
String firmware_url = "https://raw.githubusercontent.com/emhakarim/esp32-ota-proyek/main/firmware.bin";

void checkPartitionInfo() {
  Serial.println("=== Partition Info ===");
  Serial.printf("Sketch size: %u bytes\n", ESP.getSketchSize());
  Serial.printf("Free space: %u bytes\n", ESP.getFreeSketchSpace());
  Serial.printf("Flash chip size: %u bytes\n", ESP.getFlashChipSize());
  Serial.printf("Flash chip speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.println("======================");
}

void checkForUpdate() {
  Serial.println("Checking for updates...");
  
  HTTPClient http;
  http.begin(version_url);
  http.setTimeout(15000);
  
  int httpCode = http.GET();
  Serial.print("HTTP Code: ");
  Serial.println(httpCode);
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    payload.trim();
    
    Serial.print("Server version: ");
    Serial.println(payload);
    Serial.print("Current version: ");
    Serial.println(FIRMWARE_VERSION);
    
    int serverVersion = payload.toInt();
    if (serverVersion > FIRMWARE_VERSION) {
      Serial.println("New version found! Starting OTA...");
      
      WiFiClient client;
      httpUpdate.rebootOnUpdate(true);
      
      t_httpUpdate_return ret = httpUpdate.update(client, firmware_url);

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("OTA failed! Error (%d): %s\n", 
                       httpUpdate.getLastError(), 
                       httpUpdate.getLastErrorString().c_str());
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No update available.");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("OTA successful! Device will restart...");
          break;
      }
    } else {
      Serial.println("Already on latest version.");
    }
  } else {
    Serial.printf("Version check failed. HTTP: %d\n", httpCode);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== ESP32 OTA Setup ===");
  Serial.print("Firmware Version: ");
  Serial.println(FIRMWARE_VERSION);
  
  checkPartitionInfo();
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("📱 IP: ");
    Serial.println(WiFi.localIP());
    
    delay(2000);
    checkForUpdate();
  } else {
    Serial.println("\n❌ WiFi Failed!");
  }
}

void loop() {
  Serial.println("🚀 Version 2 running - UPDATED SUCCESS!");  // ⭐ UBAH PESAN INI
  delay(2000);  // ⭐ UBAH DELAY MENJADI 2 DETIK
}