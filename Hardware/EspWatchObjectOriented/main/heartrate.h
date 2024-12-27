#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <MAX3010x.h>
#include "heartratefilters.h"
#include <limits>

class HEARTRATE {
    
public:
    HEARTRATE();
    bool begin();
    void loop();
    bool pulseDetected();
    uint8_t getAvgPulse();
    uint8_t getAvgSpO2();
    bool wristDetected();


    bool enableSensor();
    bool disableSensor();
    bool setMode(const char* mode);
    bool setSamplingRate(uint16_t rate);
    bool setSampleAveraging(uint8_t samples);
    bool setLedCurrent(const char* led, float current);
    bool setPulseWidth(uint16_t pulseWidth);


    bool sensorEnabled = false;

    // Wrist Detection threshold and cooldown
    const unsigned long kDetectionThreshold = 10000;          // Change this to adjust sensitivity      // TODO: Implement DetectionThreshold calibration/auto-adjustment into the settings app
    const unsigned int kDetectionCooldownMs = 500;
    
private:
    bool wrist = false;


    // Sensor
    MAX30102 obj;

    // Sampling Rate
    // Possible sampling rates: 50, 100, 200, 400, 800, 1000, 1600, 3200
    const MAX30102::SamplingRate kSamplingRate = MAX30102::SAMPLING_RATE_400SPS;                        // TODO: Test different sampling rates to find the lowest possible rate for power saving with good accuracy
    const float kSamplingFrequency = 400.0;                                                             // TODO: Investigate optimal sampling frequency, is it kSamplingRate = kSamplingFrequency?

    // Timestamp for wrist detection
    long detection_timestamp = 0;
    bool wrist_detected = false;
    long last_pulse_detected = 0;

    // Heartbeat detection
    // Timestamp of the last heartbeat
    long last_heartbeat = 0;
    // Edge Detection Threshold
    const float kEdgeThreshold = -2000.0;
    // Last diff to detect zero crossing
    float last_diff = NAN;
    bool crossed = false;
    long crossed_time = 0;


    // Averaging
    const bool kEnableAveraging = true;
    static constexpr int kAveragingSamples = 5;
    static constexpr int kSampleThreshold = 5;
    uint8_t average_bpm;
    uint8_t average_r;
    uint8_t average_spo2;


    // Filters
    static constexpr float kLowPassCutoff = 5.0;
    static constexpr float kHighPassCutoff = 0.5;
    // Filter Instances
    LowPassFilter low_pass_filter_red;
    LowPassFilter low_pass_filter_ir;
    HighPassFilter high_pass_filter;
    Differentiator differentiator;
    MovingAverageFilter<kAveragingSamples> averager_bpm;
    MovingAverageFilter<kAveragingSamples> averager_r;
    MovingAverageFilter<kAveragingSamples> averager_spo2;
    
    

    // Spo2

    // Statistic for pulse oximetry
    MinMaxAvgStatistic stat_red;
    MinMaxAvgStatistic stat_ir;

    // R value to SpO2 calibration factors
    // See https://www.maximintegrated.com/en/design/technical-documents/app-notes/6/6845.html
    float kSpO2_A = 1.5958422;
    float kSpO2_B = -34.6596622;
    float kSpO2_C = 112.6898759;




    // TODO: Investigate the effects of different LED current values on power consumption and sensor accuracy, implement into the settings app (with calibration/auto-adjustment?)

    // TODO: Investigate the effects of LED pulse width control on power consumption and sensor accuracy, implement into the settings app (with calibration/auto-adjustment?)
    //  Resolution(bits) change with the change of pulse width, investigate the effects to accuracy
};

#endif