#include <Arduino.h>
#include <HTS221Sensor.h>
#include <Wire.h>
#include <WiFi.h>

#include "constants.h"
#include "WeatherStation.h"
#include "ESPTelemetry.h"
#include "OTAManager.h"

WeatherStation* weatherStation;
ESPTelemetry* espTelemetry;
OTAManager* otaManager;

unsigned long previousMillis;

void setup() {
  // Initialize serial for output.
  Serial.begin(115200);
  Serial.println("Starting program");
  
  Serial.println("Connecting to wifi");
  WiFi.begin(ssid, password);
  Serial.println("");

  //Wait for WIFI connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  weatherStation = WeatherStation::getInstance();
  SimplePrometheusExporter* exporter = weatherStation->getSimplePrometheusExporter();
  espTelemetry = new ESPTelemetry(exporter);
  otaManager = new OTAManager(otaPassword);

  previousMillis = millis();
}

void loop() {
  weatherStation->handleClients();
  otaManager->handle();

  if(PRINT_STATS_TO_CONSOLE) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500) {
      weatherStation->printStatsToConsole();
      previousMillis = currentMillis;
    }
  }
}
