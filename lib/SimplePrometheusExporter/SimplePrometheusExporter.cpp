#include "SimplePrometheusExporter.h"

SimplePrometheusExporterMetric::SimplePrometheusExporterMetric(String description, SimplePrometheusExporterMetricType type, FloatSupplier valueSupplier) {
    this->description = description;
    this->type = type;
    this->valueSupplier = valueSupplier;
}

SimplePrometheusExporterMetric::~SimplePrometheusExporterMetric() {

}

void SimplePrometheusExporter::putMetric(String name, SimplePrometheusExporterMetric* metric) {
    this->map[name] = metric;
}

String* SimplePrometheusExporter::getMetricString() {
    String* msg = new String("");
    for (std::pair<String, SimplePrometheusExporterMetric*> const& x : this->map)
    {
        //# HELP mpl3115a2_pressure Pressure according to the MPL3115A2 (Pa)
        //# TYPE mpl3115a2_pressure gauge
        //mpl3115a2_pressure 100764.5
        *msg += "# HELP ";
        *msg += x.first;
        *msg += " ";
        *msg += x.second->description;
        *msg += "\n# TYPE ";
        *msg += x.first;
        if(x.second->type == SimplePrometheusExporterMetricType::COUNTER) {
            *msg += " counter";
        } else {
            *msg += " gauge";
        }
        *msg += "\n";
        *msg += x.first;
        *msg += " ";
        *msg += x.second->valueSupplier();
        *msg += "\n";
    }
    return msg;
}