
#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <Arduino.h>
#include <HTS221Sensor.h>
#include <WebServer.h>

#include "SimplePrometheusExporter.h"

class WeatherStation {
public:
    static WeatherStation* getInstance();

    void handleClients();
    void printStatsToConsole();

    float getCurrentHTS221Humidity();
    float getCurrentHTS221Temperature();

private:

    WeatherStation();
    ~WeatherStation();

    void handleRoot();
    void handleNotFound();

    static void callHandleRoot();
    static void callHandleNotFound();
    static float callGetCurrentHTS221Humidity();
    static float callGetCurrentHTS221Temperature();

    HTS221Sensor* hts221;
    TwoWire* i2c;
    SimplePrometheusExporter* simplePrometheusExporter;
  
    float currHTS221Humidity;
    float currHTS221Temperature;

    static WebServer* server;
    static WeatherStation* instance;
};

#endif
