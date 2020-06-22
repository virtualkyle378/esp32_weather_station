
#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <Arduino.h>
#include <HTS221Sensor.h>
#include <Adafruit_MPL3115A2.h>
#include <WebServer.h>

#include "SimplePrometheusExporter.h"

class WeatherStation {
public:
    static void initInstance(bool enableMpl311a2);
    static WeatherStation* getInstance();

    void handleClients();
    void printStatsToConsole();

    float getCurrentHTS221Humidity();
    float getCurrentHTS221Temperature();
    float getCurrentMpl311a2Pressure();
    float getCurrentMpl311a2Temperature();

    SimplePrometheusExporter* getSimplePrometheusExporter();

private:

    WeatherStation(bool enableMpl311a2);
    virtual ~WeatherStation();

    void handleRoot();
    void handleNotFound();
    void handleDiagnostics();

    static void callHandleRoot();
    static void callHandleNotFound();
    static void callHandleDiagnostics();
    static float callGetCurrentHTS221Humidity();
    static float callGetCurrentHTS221Temperature();
    static float callGetCurrentMpl311a2Pressure();
    static float callGetCurrentMpl311a2Temperature();

    // Instances
    HTS221Sensor* hts221;
    Adafruit_MPL3115A2* mpl311a2;
    TwoWire* i2c;
    SimplePrometheusExporter* simplePrometheusExporter;
    
    // Parameters
    bool enableMpl311a2;
  
    // Current values
    float currHTS221Humidity;
    float currHTS221Temperature;
    float currMpl311a2Pressure;
    float currMpl311a2Temperature;
    
    // Diagnostic values
    int hts221_enable_status;
    float rawCurrHTS221Humidity;
    int rawCurrHTS221HumidityResultCode;
    float rawCurrHTS221Temperature;
    int rawCurrHTS221TemperatureResultCode;
    
    bool mpl311a2_begin_success;

    static WebServer* server;
    static WeatherStation* instance;
};

#endif
