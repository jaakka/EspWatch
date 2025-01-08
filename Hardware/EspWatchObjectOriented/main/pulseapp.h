#ifndef PULSEAPP_H
#define PULSEAPP_H

#include "app.h"

class PulseApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(float scale) override; 
    void drawApplicationIcon(float scale) override; 
    bool areAvailableOnMenu() override;
};

#endif 