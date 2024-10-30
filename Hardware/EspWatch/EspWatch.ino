// EspWatch project. File created 1.10.2024
// Config file

#include <Arduino_LSM6DS3.h> // Gyroscope 
#include <TFT_eSPI.h> // Lcd
#include <MAX3010x.h> // Heartrate
#include "HeartRateFilters.h" 
#define GYRO IMU

#define LCD_BACKLIGHT 32
#define VOLTAGE 35
#define BTN_DOWN 4
#define BTN_UP 23

// HEARTRATE SENSOR 
// Sensor (adjust to your sensor type)
MAX30105 HEARTRATE;
const auto kSamplingRate = HEARTRATE.SAMPLING_RATE_3200SPS;
const float kSamplingFrequency = 400.0;

// Finger Detection Threshold and Cooldown
const unsigned long kFingerThreshold = 10000;
const unsigned int kFingerCooldownMs = 500;

// Edge Detection Threshold (decrease for MAX30100)
const float kEdgeThreshold = -2000.0;

// Filters
const float kLowPassCutoff = 5.0;
const float kHighPassCutoff = 0.5;

// Averaging
const bool kEnableAveraging = true;
const int kAveragingSamples = 8;  //Options: 1, 2, 4, 8, 16, 32
const int kSampleThreshold = 5;

//  LCD
TFT_eSPI LCD  = TFT_eSPI();      // Invoke custom library

// Gyroscope
float gyro_ax , gyro_ay, gyro_az, gyro_x, gyro_y, gyro_z;