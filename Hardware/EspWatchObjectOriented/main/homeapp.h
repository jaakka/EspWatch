#ifndef HOMEAPP_H
#define HOMEAPP_H

#include "app.h"

class HomeApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(float scale) override; 
    void drawApplicationIcon(float scale) override; 
    bool areAvailableOnMenu() override;
};

#endif 