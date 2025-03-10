#ifndef DEBUG_H
#define DEBUG_H

#include "heartrate.h"
#include "gyroscope.h"
#include "pcf.h"

class DEBUG {

public:
    DEBUG(HEARTRATE& hr, GYROSCOPE& gyro, PCF& pcf);

    void printMenu();
    void loop();


private:
    HEARTRATE& heartrate;
    GYROSCOPE& gyroscope;
    PCF& pcf;

    int action;
    int mode;
    int samplingRate;
    int sampleAveraging;
    int pulseWidth;
    int led;
    float current;
    float temperature;
    float x_Acc, y_Acc, z_Acc;
    float x_Gyro, y_Gyro, z_Gyro;
};




#endif
