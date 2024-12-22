#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <MAX3010x.h>
#include "heartratefilters.h"
#include <limits>

class HEARTRATE {
    
public:
    HEARTRATE();
    bool begin();
    bool pulseDetected();
    void loop();
    int getAvgPulse();
    bool wristDetected();
    void enableSensor();
    void disableSensor();
    bool sensorEnabled = false;

private:
    MAX30102 obj;
    bool wrist = false;
    unsigned long lastWrist = 0;

    const MAX30102::SamplingRate kSamplingRate = MAX30102::SAMPLING_RATE_3200SPS;
    const float kSamplingFrequency = 400.0;
    int avgPulse;

    const int kAveragingSamples = 50;

    // Filters
    const float kLowPassCutoff = 5.0;
    const float kHighPassCutoff = 0.5;
    

    // Finger Detection Threshold and Cooldown
    const unsigned long kFingerThreshold = 10000;
    const unsigned int kFingerCooldownMs = 500;

    long finger_timestamp = 0;
    bool finger_detected = false;
};

#endif