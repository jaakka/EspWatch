#include "pcf.h"
#include <Adafruit_PCF8591.h>


PCF::PCF() 
    : obj(Adafruit_PCF8591()) {}


bool PCF::begin() {
    if (obj.begin()) 
    {
        PCF::DAC_enabled(false);
        PCF::analogWrite(255);

        Serial.println("PCF8591 initialized");
        return true;
    } 
    else 
    {
        Serial.println("PCF8591 initialization failed");
        return false;
    }
}

/**
 * @param pin Pin to read from (0-3)
 * @returns Value read from pin (0-255)
 */
uint8_t PCF::analogRead(uint8_t pin) {
    return obj.analogRead(pin);
}


/**
 * @param value Value to write to DAC (0-255)
 */
void PCF::analogWrite(uint8_t value) {
    obj.analogWrite(value);
}


/**
 * @param state Enable or disable DAC
 */
void PCF::DAC_enabled(bool state) {
    obj.enableDAC(state);
}