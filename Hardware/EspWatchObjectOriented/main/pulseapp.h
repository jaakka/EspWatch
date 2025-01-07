#ifndef PULSEAPP_H
#define PULSEAPP_H

#include "app.h"

class PulseApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication() override; 
    void drawApplicationIcon() override; 
};

#endif 