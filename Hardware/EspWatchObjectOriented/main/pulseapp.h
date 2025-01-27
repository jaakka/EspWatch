#ifndef PULSEAPP_H
#define PULSEAPP_H

#include "app.h"

class PulseApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(int x, int y, float scale) override; 
    void drawApplicationIcon(int x, int y, float scale) override; 
    bool areAvailableOnMenu() override;
private:
    void pulseAnimation();
    void handleApplicationAllTime();
    void drawHeartIcon(float x, float y, float heart_scale);
    int pulseAnim;
};

#endif 