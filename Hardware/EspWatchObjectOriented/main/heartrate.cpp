#include "MAX30102.h"
#include "heartrate.h"
#include <Arduino.h>
#include "heartratefilters.h"


HEARTRATE::HEARTRATE() 
  : obj(MAX30102()),
    // Filter Instances
    low_pass_filter_red(HEARTRATE::kLowPassCutoff, kSamplingFrequency),
    low_pass_filter_ir(HEARTRATE::kLowPassCutoff, kSamplingFrequency),
    high_pass_filter(HEARTRATE::kHighPassCutoff, kSamplingFrequency),
    differentiator(kSamplingFrequency),
    averager_bpm(),
    averager_r(),
    averager_spo2() {}

bool HEARTRATE::begin() {
    if (obj.begin()) 
    {
        if (obj.setSamplingRate(kSamplingRate)) 
        {
            Serial.println("Sensor initialized");
        }
        return true;
    } 
    else 
    {
        return false;
    }
}

void HEARTRATE::enableSensor() {
    // TODO: Implement enableSensor() 
    sensorEnabled = true;
}

void HEARTRATE::disableSensor() {
    // TODO: Implement disableSensor()
    sensorEnabled = false;
}

bool HEARTRATE::pulseDetected() {
    return wrist_detected; //THIS IS TEST, IF WRIST DETECTED MAKE PULSE ANIMATIONS
}

uint8_t HEARTRATE::getAvgPulse() {
    // TODO: Maybe add functionality e.g. getAvgPulse(minute), getAvgPulse(hour) etc? Do the same for SpO2
    return average_bpm;
}
uint8_t HEARTRATE::getAvgSpO2() {
    return average_spo2;
}

bool HEARTRATE::wristDetected() { //käyttäjän ranne tunnistettu
      return wrist_detected;
}




void HEARTRATE::loop() {
  auto sample = obj.readSample(1000);
  float current_value_red = sample.red;
  float current_value_ir = sample.ir;
  //avgPulse = current_value;

  // Detect wrist using raw sensor value
  if(sample.red > kDetectionThreshold) {
    if(millis() - detection_timestamp > kDetectionCooldownMs) {
      wrist_detected = true;
    }
  }
  else {
    // Reset values if the finger is removed
    differentiator.reset();
    averager_bpm.reset();
    averager_r.reset();
    averager_spo2.reset();
    low_pass_filter_red.reset();
    low_pass_filter_ir.reset();
    high_pass_filter.reset();
    stat_red.reset();
    stat_ir.reset();
    
    wrist_detected = false;
    detection_timestamp = millis();
  }

  if(wrist_detected) {
    current_value_red = low_pass_filter_red.process(current_value_red);
    current_value_ir = low_pass_filter_ir.process(current_value_ir);

    // Statistics for pulse oximetry
    stat_red.process(current_value_red);
    stat_ir.process(current_value_ir);

    // Heart beat detection using value for red LED
    float current_value = high_pass_filter.process(current_value_red);
    float current_diff = differentiator.process(current_value);

    // Valid values?
    if(!isnan(current_diff) && !isnan(last_diff)) {
      
      // Detect Heartbeat - Zero-Crossing
      if(last_diff > 0 && current_diff < 0) {
        crossed = true;
        crossed_time = millis();
      }
      
      if(current_diff > 0) {
        crossed = false;
      }
  
      // Detect Heartbeat - Falling Edge Threshold
      if(crossed && current_diff < kEdgeThreshold) {
        if(last_heartbeat != 0 && crossed_time - last_heartbeat > 300) {
          // Show Results
          int bpm = 60000/(crossed_time - last_heartbeat);
          float rred = (stat_red.maximum()-stat_red.minimum())/stat_red.average();
          float rir = (stat_ir.maximum()-stat_ir.minimum())/stat_ir.average();
          float r = rred/rir;
          float spo2 = kSpO2_A * r * r + kSpO2_B * r + kSpO2_C;
          
          if(bpm > 40 && bpm < 250) {
            // Average?
            // TODO: Implement averaging for different time periods, using default example averager for now
            // TODO: Delete average_r if it's useless
            // TODO: Reset averages if the finger is removed?
            if(kEnableAveraging) {
              average_bpm = averager_bpm.process(bpm);
              average_r = averager_r.process(r);
              average_spo2 = averager_spo2.process(spo2);
  
              // Show if enough samples have been collected
              if(averager_bpm.count() >= kSampleThreshold) {
                Serial.print("Time (ms): ");
                Serial.println(millis()); 
                Serial.print("Heart Rate (avg, bpm): ");
                Serial.println(average_bpm);
                Serial.print("R-Value (avg): ");
                Serial.println(average_r);  
                Serial.print("SpO2 (avg, %): ");
                Serial.println(average_spo2);
                Serial.print("Raw Red: ");
                Serial.println(current_value_red);
                Serial.println(wrist_detected);
              }
            }
            else {
              Serial.print("Time (ms): ");
              Serial.println(millis()); 
              Serial.print("Heart Rate (current, bpm): ");
              Serial.println(bpm);  
              Serial.print("R-Value (current): ");
              Serial.println(r);
              Serial.print("SpO2 (current, %): ");
              Serial.println(spo2);   
            }
          }

          // Reset statistic
          stat_red.reset();
          stat_ir.reset();
        }
  
        crossed = false;
        last_heartbeat = crossed_time;
      }
    }

    last_diff = current_diff;
  }
}
