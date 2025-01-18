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
    float near_app_x;
    float near_app_y;
    float touch_start_x;
    float touch_start_y;
    bool is_dragging;
    float application_open_scale;
    unsigned long last_touch;
    unsigned long last_release;
    int near_app_id;
    bool debug_mode;
    bool enable_cursor;
    bool application_starting;
    bool application_closing;
    void applicationClosed();
    int checkApplicationId(int id);
    void smoothMove(float &current_x, float &current_y, float &target_x, float &target_y);
    //void getNearestApplication(const float &current_x, const float &current_y ,float &application_x, float &application_y, int &application_id);
};

#endif 

