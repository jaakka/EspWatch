// EspWatch project. File created 1.10.2024
// Config file

#include <Arduino_LSM6DS3.h> // Gyroscope 
#include <TFT_eSPI.h> // Lcd
#include <MAX3010x.h> // Heartrate
#include <CST816S.h>  // Touchscreen
#include "HeartRateFilters.h" 
#include <Adafruit_PCF8591.h>

#define GYRO IMU // Osoite muuttuu lopullisella tytärlevyllä B > A
#define LCD_BACKLIGHT 32
#define VOLTAGE 35
#define BTN_DOWN 4
#define BTN_UP 23

#define TOUCH_SDA 21
#define TOUCH_SCL 22
#define TOUCH_RST 4
#define TOUCH_INT 19

unsigned long last_write;
unsigned long last_pcf;

String last_print = "";
String last_print2 = "";
String last_print3 = "";
String last_print4 = "";

int avg_pulse=0, pulse=0;
int avg_red=0, red=0;
int avg_spo=0, spo=0;

// HEARTRATE SENSOR 
// Sensor (adjust to your sensor type)
MAX30102 HEARTRATE;
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

// Filter Instances
LowPassFilter low_pass_filter_red(kLowPassCutoff, kSamplingFrequency);
LowPassFilter low_pass_filter_ir(kLowPassCutoff, kSamplingFrequency);
HighPassFilter high_pass_filter(kHighPassCutoff, kSamplingFrequency);
Differentiator differentiator(kSamplingFrequency);
MovingAverageFilter<kAveragingSamples> averager_bpm;
MovingAverageFilter<kAveragingSamples> averager_r;
MovingAverageFilter<kAveragingSamples> averager_spo2;

// Statistic for pulse oximetry
MinMaxAvgStatistic stat_red;
MinMaxAvgStatistic stat_ir;

// R value to SpO2 calibration factors
// See https://www.maximintegrated.com/en/design/technical-documents/app-notes/6/6845.html
float kSpO2_A = 1.5958422;
float kSpO2_B = -34.6596622;
float kSpO2_C = 112.6898759;

// Timestamp of the last heartbeat
long last_heartbeat = 0;

// Timestamp for finger detection
long finger_timestamp = 0;
bool finger_detected = false;

// Last diff to detect zero crossing
float last_diff = NAN;
bool crossed = false;
long crossed_time = 0;

//  LCD
TFT_eSPI LCD  = TFT_eSPI();      // Invoke custom library

CST816S touch(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);

// Gyroscope
float gyro_ax , gyro_ay, gyro_az, gyro_x, gyro_y, gyro_z;

// PCF8591
Adafruit_PCF8591 pcf = Adafruit_PCF8591();