
#include "ESPTelemetry.h"

float getESPHeapSize() {
  return ESP.getHeapSize();
}

float getESPFreeHeap() {
  return ESP.getFreeHeap();
}

float getESPCpuFreqMHz() {
  return ESP.getCpuFreqMHz();
}

float getESPPsramSize() {
  return ESP.getPsramSize();
}

float getESPUptimeSeconds() {
  return millis() / 1000.0;
}

float getESPWifiChannel() {
  return WiFi.channel();
}

float getESPWifiRSSI() {
  return WiFi.RSSI();
}

ESPTelemetry::ESPTelemetry(SimplePrometheusExporter* exporter) {
  exporter->putMetric("heap_size", new SimplePrometheusExporterMetric(
      "Heap size (bytes)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPHeapSize
  ));
  exporter->putMetric("heap_free", new SimplePrometheusExporterMetric(
      "Heap free space (bytes)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPFreeHeap
  ));
  exporter->putMetric("cpu_freq", new SimplePrometheusExporterMetric(
      "CPU Freq (MHz)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPCpuFreqMHz
  ));
  exporter->putMetric("psram_size", new SimplePrometheusExporterMetric(
      "psram size (bytes)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPPsramSize
  ));
  exporter->putMetric("uptime", new SimplePrometheusExporterMetric(
      "Uptime (sec)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPUptimeSeconds
  ));
  exporter->putMetric("wifi_channel", new SimplePrometheusExporterMetric(
      "WiFi channel",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPWifiChannel
  ));
  exporter->putMetric("wifi_rssi", new SimplePrometheusExporterMetric(
      "WiFi Received Signal Strength Indicator (RSSI)",
      SimplePrometheusExporterMetricType::GAUGE,
      getESPWifiRSSI
  ));
}

ESPTelemetry::~ESPTelemetry() {

}