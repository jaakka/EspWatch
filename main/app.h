#ifndef APP_H
#define APP_H

#include <TFT_eSPI.h>
#include "touch.h"
#include "utils.h"

extern TFT_eSprite frame;
extern TOUCH touch;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern int runningApp;
extern bool exit_application;

class App {
public:
    virtual void init() = 0;                            // Setup application values
    virtual void handleApplication() = 0;               // Handle application logic & touch 
    virtual void handleApplicationBackground() = 0;     // Handle application background logic if need run background  
    virtual void drawApplication(int x = 0, int y = 0, float scale = 10) = 0; // Draw application on screen
    virtual void drawApplicationIcon(int x, int y, float scale = 2.8) = 0;  // Draw application icon on menu
    virtual bool areAvailableOnMenu() = 0;              // Return true if application are available on menu
};

#endif // APP_H
