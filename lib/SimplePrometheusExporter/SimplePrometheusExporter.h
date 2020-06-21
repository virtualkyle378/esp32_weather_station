
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

    void putMetric(String name, SimplePrometheusExporterMetric* metric);
    String* getMetricString();

private:

    std::map<String, SimplePrometheusExporterMetric*> map;

};

#endif
