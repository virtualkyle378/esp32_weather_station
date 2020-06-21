
#ifndef SIMPLEPROMETHEUSEXPORTER_H
#define SIMPLEPROMETHEUSEXPORTER_H

#include <Arduino.h>
#include <map>
#include <string>

typedef float (*FloatSupplier)(); 

enum class SimplePrometheusExporterMetricType {
    COUNTER,
    GAUGE
};

class SimplePrometheusExporterMetric {
    friend class SimplePrometheusExporter;

public:
    SimplePrometheusExporterMetric(String description, SimplePrometheusExporterMetricType type, FloatSupplier valueSupplier);
    virtual ~SimplePrometheusExporterMetric();

private:
    String description;
    SimplePrometheusExporterMetricType type;
    FloatSupplier valueSupplier;
};

class SimplePrometheusExporter {
public:

    SimplePrometheusExporter();
    virtual ~SimplePrometheusExporter();

    void putMetric(String name, SimplePrometheusExporterMetric* metric);
    const char* getMetricString();

private:

    std::map<String, SimplePrometheusExporterMetric*>* map;
    std::string* msg;

};

#endif
