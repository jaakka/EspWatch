#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <Arduino_LSM6DS3.h>

class GYROSCOPE {
public:
    GYROSCOPE();
    bool begin();
    void loop();


    bool readAcceleration(float& x_Acc, float& y_Acc, float& z_Acc);
    float getAccelerationSampleRate();
    bool accelerationAvailable();

    bool readGyroscope(float& x, float& y, float& z);
    float getGyroscopeSampleRate();
    bool gyroscopeAvailable();

    bool readTemperature(float& t);
    float getTemperatureSampleRate();
    bool temperatureAvailable();

private:

    // Sensor
    LSM6DS3Class obj;

};

#endif 

// TODO: Implement calibration
// TODO: Add functionality for setting sample rate, sleep mode etc. using writeRegister()
