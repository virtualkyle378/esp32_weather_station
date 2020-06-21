
#ifndef ESPTELEMETRY_H
#define ESPTELEMETRY_H

#include <Arduino.h>
#include <WiFi.h>
#include "SimplePrometheusExporter.h"

class ESPTelemetry {
public:
  ESPTelemetry(SimplePrometheusExporter* exporter);
  virtual ~ESPTelemetry();
};

#endif