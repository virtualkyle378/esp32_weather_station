#include <Arduino.h>
#include <HTS221Sensor.h>
#include <Wire.h>
#include <WiFi.h>

#include "global_constants.h"
#include "device_constants.h"
#include "WeatherStation.h"
#include "ESPTelemetry.h"
#include "OTAManager.h"

WeatherStation* weatherStation;
ESPTelemetry* espTelemetry;
OTAManager* otaManager;

unsigned long previousMillis;
unsigned long lastWifiCheck = 30000;

void setup() {
  // Initialize serial for output.
  Serial.begin(115200);
  Serial.println("Starting program");
  
  Serial.println("Connecting to WiFi");
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  WiFi.disconnect(true, true);
  WiFi.setAutoReconnect(true);

  // Workaround for bug in WiFi class
  // See https://github.com/espressif/arduino-esp32/issues/806#issuecomment-619982605
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);

  uint8_t status = WiFi.begin(ssid, password);
  Serial.println("Wifi.begin returned ");
  Serial.print(status);

  Serial.print("Setting hostname... ");
  bool success = WiFi.setHostname(hostname);
  if(success) {
    Serial.println("succeeded");
  } else {
    Serial.println("failed");
  }

  Serial.println("");
  
  // Wait for WIFI connection
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
    Serial.print(WiFi.status());
    Serial.print(WiFi.reconnect());
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  WeatherStation::initInstance(enableMpl311a2);
  weatherStation = WeatherStation::getInstance();
  SimplePrometheusExporter* exporter = weatherStation->getSimplePrometheusExporter();
  espTelemetry = new ESPTelemetry(exporter);
  otaManager = new OTAManager(otaPassword);

  previousMillis = millis();
}

void loop() {
  weatherStation->handleClients();
  otaManager->handle();

  if ((millis() > lastWifiCheck) && (WiFi.status() != WL_CONNECTED)) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    lastWifiCheck = millis() + 5000;
  }

  if(PRINT_STATS_TO_CONSOLE) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500) {
      weatherStation->printStatsToConsole();
      previousMillis = currentMillis;
    }
  }
}
