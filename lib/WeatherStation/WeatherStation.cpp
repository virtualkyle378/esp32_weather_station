
#include "WeatherStation.h"

WebServer* WeatherStation::server = nullptr;
WeatherStation* WeatherStation::instance = nullptr;

WeatherStation* WeatherStation::getInstance() {
  if(instance == nullptr) {
    instance = new WeatherStation();
  }
  return instance;
}

WeatherStation::WeatherStation() {
  // Initialize I2C bus.
  i2c = new TwoWire(0);
  i2c->begin();
  
  hts221 = new HTS221Sensor(i2c);
  hts221->Enable();

  simplePrometheusExporter = new SimplePrometheusExporter();
  simplePrometheusExporter->putMetric("hts221_temperature", new SimplePrometheusExporterMetric(
    "Temperature according to the HTS221 (C)",
    SimplePrometheusExporterMetricType::GAUGE,
    callGetCurrentHTS221Temperature
  ));
  simplePrometheusExporter->putMetric("hts221_relative_humidity", new SimplePrometheusExporterMetric(
    "Relative humidity according to the HTS221 (rH)",
    SimplePrometheusExporterMetricType::GAUGE,
    callGetCurrentHTS221Humidity
  ));

  server = new WebServer(80);  // Object of WebServer(HTTP port, 80 is defult)

  server->on("/", WeatherStation::callHandleRoot);
  server->on("/metrics", WeatherStation::callHandleRoot);
  server->onNotFound( WeatherStation::callHandleNotFound);

  server->begin();
}

WeatherStation::~WeatherStation() {
  delete i2c;
  delete hts221;
  delete simplePrometheusExporter;
  delete server;
}

void WeatherStation::callHandleRoot() {
  WeatherStation::instance->handleRoot();
}

void WeatherStation::callHandleNotFound() {
  WeatherStation::instance->handleNotFound();
}

void WeatherStation::handleRoot() {
  server->send(200, "text/plain", String(simplePrometheusExporter->getMetricString()));
}

void WeatherStation::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/html", message);
}

void WeatherStation::handleClients() {
  server->handleClient();
}

void WeatherStation::printStatsToConsole() {
  // Output data.
  Serial.print("Hum[%]: ");
  Serial.print(getCurrentHTS221Humidity(), 2);
  Serial.print(" | Temp[C]: ");
  Serial.println(getCurrentHTS221Temperature(), 2);
}
    
float WeatherStation::getCurrentHTS221Humidity() {
  if(hts221->GetHumidity(&currHTS221Humidity) != HTS221_STATUS_OK) {
    currHTS221Humidity = NAN;
  }
  return currHTS221Humidity;
}

float WeatherStation::callGetCurrentHTS221Humidity() {
  return instance->getCurrentHTS221Humidity();
}

float WeatherStation::getCurrentHTS221Temperature() {
  if(hts221->GetTemperature(&currHTS221Temperature) != HTS221_STATUS_OK) {
    currHTS221Temperature = NAN;
  }
  return currHTS221Temperature;
}

float WeatherStation::callGetCurrentHTS221Temperature() {
  return instance->getCurrentHTS221Temperature();
}