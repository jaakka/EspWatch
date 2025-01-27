#ifndef PCF_H
#define PCF_H

#include <Adafruit_PCF8591.h>

class PCF {
public:
    PCF();
    bool begin();

    uint8_t analogRead(uint8_t pin);
    void analogWrite(uint8_t value);

    void DAC_enabled(bool state);


    // Quick functions for the light sensor

    void enableLightSensor();
    void disableLightSensor();

    uint8_t readHallSensor1();
    uint8_t readHallSensor2();
    uint8_t readLightSensor();

private:

    Adafruit_PCF8591 obj;

};

#endif
