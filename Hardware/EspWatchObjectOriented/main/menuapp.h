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
    float menu_xpos;
    float menu_ypos;
    float target_xpos;
    float target_ypos;
    float near_app_x;
    float near_app_y;
    float touch_start_x;
    float touch_start_y;
    bool isDragging;
    float openApplicationScale;
    unsigned long last_touch;
    unsigned long last_release;
    int checkApplicationId(int id);
    void smoothMove(float *current_x, float *current_y, float target_x, float target_y);
    void getNearestApplication(float *near_app_x, float *near_app_y, float x_offset, float y_offset, float scale);
};

#endif 

