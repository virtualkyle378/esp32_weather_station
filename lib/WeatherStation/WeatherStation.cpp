
#include "WeatherStation.h"

WebServer* WeatherStation::server = nullptr;
WeatherStation* WeatherStation::instance = nullptr;

void WeatherStation::initInstance(bool enableMpl311a2) {
  if(instance == nullptr) {
    instance = new WeatherStation(enableMpl311a2);
  } else {
    Serial.println("ERROR: WeatherStation is already initalized");
  }
}

WeatherStation* WeatherStation::getInstance() {
  if(instance == nullptr) {
    Serial.println("You need to call WeatherStation::initInstance() before WeatherStation::getInstance()");
    delay(1000);
    ESP.restart();
  }
  return instance;
}

WeatherStation::WeatherStation(bool enableMpl311a2) {
  this->enableMpl311a2 = enableMpl311a2;

  // Initialize I2C bus.
  i2c = new TwoWire(0);
  i2c->begin();
  
  hts221 = new HTS221Sensor(i2c);
  hts221_enable_status = hts221->Enable();

  mpl311a2 = new Adafruit_MPL3115A2();
  if(enableMpl311a2) {
    mpl311a2_begin_success = mpl311a2->begin(i2c);
  }

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
    
  if(enableMpl311a2) {
    simplePrometheusExporter->putMetric("mpl3115a2_pressure", new SimplePrometheusExporterMetric(
      "Pressure according to the MPL3115A2 (Pa)",
      SimplePrometheusExporterMetricType::GAUGE,
      callGetCurrentMpl311a2Pressure
    ));
    simplePrometheusExporter->putMetric("mpl3115a2_temperature", new SimplePrometheusExporterMetric(
      "Temperature according to the MPL3115A2 (C)",
      SimplePrometheusExporterMetricType::GAUGE,
      callGetCurrentMpl311a2Temperature
    ));
  }

  server = new WebServer(8000);  // Object of WebServer(HTTP port, 80 is defult)

  server->on("/", WeatherStation::callHandleRoot);
  server->on("/metrics", WeatherStation::callHandleRoot);
  server->on("/diag", WeatherStation::callHandleDiagnostics);
  server->on("/diagnostics", WeatherStation::callHandleDiagnostics);
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

void WeatherStation::callHandleDiagnostics() {
  WeatherStation::instance->handleDiagnostics();
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

void WeatherStation::handleDiagnostics() {
  String message = "Diagnostics";
  message += "\nhts221_enable_status: ";
  message += hts221_enable_status;
  message += "\nrawCurrHTS221Humidity: ";
  message +=  rawCurrHTS221Humidity;
  message += "\nrawCurrHTS221HumidityResultCode: ";
  message +=  rawCurrHTS221HumidityResultCode;
  message += "\nrawCurrHTS221Temperature: ";
  message +=  rawCurrHTS221Temperature;
  message += "\nrawCurrHTS221TemperatureResultCode: ";
  message +=  rawCurrHTS221TemperatureResultCode;
  if(enableMpl311a2) {
    message += "\nmpl311a2_begin_success: ";
    message += mpl311a2_begin_success;
  }
  server->send(200, "text/plain", message);
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
  
  if(enableMpl311a2) {
    Serial.print("pressure[p] ");
    Serial.print(mpl311a2->getPressure(), 2);
    Serial.print(" | Temp[C]: ");
    Serial.println(mpl311a2->getTemperature(), 2);
  }
}
    
float WeatherStation::getCurrentHTS221Humidity() {
  int resultCode = hts221->GetHumidity(&currHTS221Humidity);
  rawCurrHTS221Humidity = currHTS221Humidity;
  rawCurrHTS221HumidityResultCode = resultCode;
  if(resultCode != HTS221_STATUS_OK || currHTS221Humidity == 6553.5) {
    currHTS221Humidity = NAN;
  }
  return currHTS221Humidity;
}

float WeatherStation::callGetCurrentHTS221Humidity() {
  return instance->getCurrentHTS221Humidity();
}

float WeatherStation::getCurrentHTS221Temperature() {
  int resultCode = hts221->GetTemperature(&currHTS221Temperature);
  rawCurrHTS221Temperature = currHTS221Temperature;
  rawCurrHTS221TemperatureResultCode = resultCode;
  if(resultCode != HTS221_STATUS_OK) {
    currHTS221Temperature = NAN;
  }
  return currHTS221Temperature;
}

float WeatherStation::callGetCurrentHTS221Temperature() {
  return instance->getCurrentHTS221Temperature();
}

float WeatherStation::getCurrentMpl311a2Pressure() {
  return mpl311a2->getPressure();
}

float WeatherStation::callGetCurrentMpl311a2Pressure() {
  return instance->getCurrentMpl311a2Pressure();
}

float WeatherStation::getCurrentMpl311a2Temperature() {
  return mpl311a2->getTemperature();
}

float WeatherStation::callGetCurrentMpl311a2Temperature() {
  return instance->getCurrentMpl311a2Temperature();
}


SimplePrometheusExporter* WeatherStation::getSimplePrometheusExporter() {
  return simplePrometheusExporter;
}