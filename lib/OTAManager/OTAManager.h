
#ifndef OTAMANAGER_H
#define OTAMANAGER_H

#include <ArduinoOTA.h>

class OTAManager {
public:
    OTAManager(const char* password);
    virtual ~OTAManager();

    void handle();
};

#endif
