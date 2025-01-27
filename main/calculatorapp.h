#ifndef CALCULATORAPP_H
#define CALCULATORAPP_H

#include "app.h"

class CalculatorApp : public App {
public:
    void init() override;      
    void handleApplication() override;        
    void handleApplicationBackground() override;        
    void drawApplication(int x, int y, float scale) override; 
    void drawApplicationIcon(int x, int y, float scale) override; 
    bool areAvailableOnMenu() override;

private:
    String enteredNumber;
    String savedNumber1;
    String savedNumber2;
    String operatorSymbol;
    bool isEnteringNumber;
    bool numberEntered;
    bool wasTouched;
    int currentScreen;
    bool showresult;
};

#endif 
