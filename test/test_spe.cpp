#include <Arduino.h>
#include <unity.h>

#include <SimplePrometheusExporter.h>

float currentPressure;
float currentTemperature;

float getCurrentPressure() {
    return currentPressure;
}

float getCurrentTemperature() {
    return currentTemperature;
}

void test_spe(void) {

    SimplePrometheusExporter* spe = new SimplePrometheusExporter();

    spe->putMetric("mpl3115a2_pressure", new SimplePrometheusExporterMetric(
        "Pressure according to the MPL3115A2 (Pa)",
        SimplePrometheusExporterMetricType::GAUGE,
        getCurrentPressure));
    
    spe->putMetric("mpl3115a2_temperature", new SimplePrometheusExporterMetric(
        "Temperature according to the MPL3115A2 (C)",
        SimplePrometheusExporterMetricType::GAUGE,
        getCurrentTemperature));

    currentPressure = 100764.5;
    currentTemperature = 22.75;

    String expected = "";
    expected += "# HELP mpl3115a2_pressure Pressure according to the MPL3115A2 (Pa)\n";
    expected += "# TYPE mpl3115a2_pressure gauge\n";
    expected += "mpl3115a2_pressure 100764.50\n";
    expected += "# HELP mpl3115a2_temperature Temperature according to the MPL3115A2 (C)\n";
    expected += "# TYPE mpl3115a2_temperature gauge\n";
    expected += "mpl3115a2_temperature 22.75\n";

    const char* actual = spe->getMetricString();
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), String(actual).c_str());

    delete spe;
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_spe);
    UNITY_END();
}

void loop() {

}