#ifndef HOMEAPP_H
#define HOMEAPP_H

#include "app.h"
#include <TimeLib.h>

class HomeApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(int x, int y, float scale) override; 
    void drawApplicationIcon(int x, int y, float scale) override; 
    bool areAvailableOnMenu() override;
private: 
    void drawAnalogWatch(int x, int y, float scale);
    void drawDigitalWatch(int x, int y, float scale);
    String addZerosIfNeeded(int value);
    String digitalTime();
    unsigned long last_release;
    bool edit_mode;
    float last_angle;
    bool analog_watch;
};

#endif 