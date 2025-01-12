#ifndef MENUAPP_H
#define MENUAPP_H

#include "app.h"

extern App* apps[]; // Get applications from main
extern const int APP_COUNT;

class MenuApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(int x, int y, float scale) override; 
    void drawApplicationIcon(int x, int y, float scale) override; 
    bool areAvailableOnMenu() override;
private:
    float menu_xpos = 0;
    float menu_ypos = 0;
    float target_xpos = 0;
    float target_ypos = 0;
    bool isDragging = false;
    int checkApplicationId(int id);
    void smoothMove(int* current_x, int* current_y, int target_x, int target_y);
};

#endif 

