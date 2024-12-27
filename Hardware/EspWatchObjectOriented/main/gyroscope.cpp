#include "gyroscope.h"
#include <Arduino_LSM6DS3.h>


GYROSCOPE::GYROSCOPE()
    : obj(Wire, LSM6DS3_ADDRESS) {}


bool GYROSCOPE::begin() {
    if (obj.begin()) 
    {
        Serial.println("Sensor initialized");
        return true;
    } 
    else 
    {
        return false;
    }
}


// TODO: sample rate returns are hardcoded to "return XXX.0F" in the library, modify the library to return the actual sample rates or implement it here
/**
 * @note Acceleration readings are in g (earth gravity)
 * @returns true if successful, false if failed
 */
bool GYROSCOPE::readAcceleration(float& x_Acc, float& y_Acc, float& z_Acc) {
    return obj.readAcceleration(x_Acc, y_Acc, z_Acc);
}

/**
 * @returns Sample rate as float UNINPLEMENTED IN LIBRARY
 */
float GYROSCOPE::getAccelerationSampleRate() {
    return obj.accelerationSampleRate();
}

bool GYROSCOPE::accelerationAvailable() {
    return obj.accelerationAvailable();
}


/**
 * @note Gyroscope readings are in degrees/second
 * @returns true if successful, false if failed
 */
bool GYROSCOPE::readGyroscope(float& x_Gyro, float& y_Gyro, float& z_Gyro) {
    return obj.readGyroscope(x_Gyro, y_Gyro, z_Gyro);
}

/**
 * @returns Sample rate as float UNINPLEMENTED IN LIBRARY
 */
float GYROSCOPE::getGyroscopeSampleRate() {
    return obj.gyroscopeSampleRate();
}

bool GYROSCOPE::gyroscopeAvailable() {
    return obj.gyroscopeAvailable();
}

/**
 * @returns temperature in degrees Celsius
 */
bool GYROSCOPE::readTemperature(float& t) {
    return obj.readTemperature(t);
}

/**
 * @returns Sample rate as float UNINPLEMENTED IN LIBRARY
 */
float GYROSCOPE::getTemperatureSampleRate() {
    return obj.temperatureSampleRate();
}

bool GYROSCOPE::temperatureAvailable() {
    return obj.temperatureAvailable();
}




void GYROSCOPE::loop() {
    
}