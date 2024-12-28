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

private:

    Adafruit_PCF8591 obj;

};

#endif