#ifndef APP_H
#define APP_H

class App {
public:
    virtual void init() = 0;                            // Setup application values
    virtual void handleApplication() = 0;               // Handle application logic & touch 
    virtual void handleApplicationBackground() = 0;     // Handle application background logic if need run background  
    virtual void drawApplication(float scale = 10) = 0; // Draw application on screen
    virtual void drawApplicationIcon(float scale) = 0;  // Draw application icon on menu
    virtual bool areAvailableOnMenu() = 0;              // Return true if application are available on menu
};

#endif // APP_H