#ifndef HOMEAPP_H
#define HOMEAPP_H

#include "app.h"

class HomeApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication() override; 
    void drawApplicationIcon() override; 
};

#endif 