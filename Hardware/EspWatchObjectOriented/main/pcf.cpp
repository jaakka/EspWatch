#include "pcf.h"
#include <Adafruit_PCF8591.h>


PCF::PCF() 
    : obj(Adafruit_PCF8591()) {}


bool PCF::begin() {
    if (obj.begin()) 
    {
        // Disable DAC (power to light sensor) and set it to the max value in advance for when it is enabled later
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




// Quick functions for the light sensor

void PCF::enableLightSensor() {
    obj.enableDAC(true);
}

void PCF::disableLightSensor() {
    obj.enableDAC(false);
}

/**
 * @returns Value read from Hall sensor 1 (0-255)
 */
uint8_t PCF::readHallSensor1() {
    return obj.analogRead(0);
}

/**
 * @returns Value read from Hall sensor 2 (0-255)
 */
uint8_t PCF::readHallSensor2() {
    return obj.analogRead(1);
}

/**
 * @returns Value read from light sensor (0-255)
 */
uint8_t PCF::readLightSensor() {
    return obj.analogRead(2);
}

// pin 3 is not connected