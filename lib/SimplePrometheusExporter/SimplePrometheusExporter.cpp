#include "SimplePrometheusExporter.h"

SimplePrometheusExporterMetric::SimplePrometheusExporterMetric(String description, SimplePrometheusExporterMetricType type, FloatSupplier valueSupplier) {
    this->description = description;
    this->type = type;
    this->valueSupplier = valueSupplier;
}

SimplePrometheusExporterMetric::~SimplePrometheusExporterMetric() {

}

SimplePrometheusExporter::SimplePrometheusExporter() {
    this->map = new std::map<String, SimplePrometheusExporterMetric*>();
    this->msg = new std::string();
}

SimplePrometheusExporter::~SimplePrometheusExporter() {
    delete this->map;
    delete this->msg;
}

void SimplePrometheusExporter::putMetric(String name, SimplePrometheusExporterMetric* metric) {
    (*this->map)[name] = metric;
}

const char* SimplePrometheusExporter::getMetricString() {
    this->msg->resize(0);
    for (std::pair<String, SimplePrometheusExporterMetric*> const& x : *this->map)
    {
        //# HELP mpl3115a2_pressure Pressure according to the MPL3115A2 (Pa)
        //# TYPE mpl3115a2_pressure gauge
        //mpl3115a2_pressure 100764.5
        msg->append("# HELP ");
        msg->append(x.first.c_str());
        msg->append(" ");
        msg->append(x.second->description.c_str());
        msg->append("\n# TYPE ");
        msg->append(x.first.c_str());
        if(x.second->type == SimplePrometheusExporterMetricType::COUNTER) {
            msg->append(" counter");
        } else {
            msg->append(" gauge");
        }
        msg->append("\n");
        msg->append(x.first.c_str());
        msg->append(" ");
        msg->append(String(x.second->valueSupplier()).c_str());
        msg->append("\n");
    }
    return msg->c_str();
}