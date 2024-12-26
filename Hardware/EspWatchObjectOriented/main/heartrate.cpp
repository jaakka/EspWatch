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





// TODO: Test enableSensor, disableSensor, setMode, setSamplingrate, setSampleAveraging, setLedCurrent, setPulseWidth
/**
 * @returns true if successful, false if failed
 */
bool HEARTRATE::enableSensor() {
  if (obj.wakeUp())
  {
    sensorEnabled = true;
    return true;
  }
  else
  {
    return false;
  }
}
/**
 * @returns true if successful, false if failed
 */
bool HEARTRATE::disableSensor() {
  if (obj.shutdown())
  {
    sensorEnabled = false;
    return true;
  }
  else
  {
    return false;
  }
}


/**
 * @param mode "MODE_HR_ONLY" or "MODE_SPO2", Default: "MODE_SP02"
 * @returns true if successful, false if failed
 */
bool HEARTRATE::setMode(const char *mode) {
  MAX30102::Mode modeEnum;

  if (strcmp(mode, "MODE_HR_ONLY") == 0)
  {
    modeEnum = MAX30102::MODE_HR_ONLY;
  }
  else if (strcmp(mode, "MODE_SPO2") == 0)
  {
    modeEnum = MAX30102::MODE_SPO2;
  }
  else
  {
    return false;
  }

  if (obj.setMode(modeEnum))
  {
    return true;
  }
  else
  {
    return false;
  }
}


/** 
 * @param rate Samples per second (50, 100, 200, 400, 800, 1000, 1600, 3200)
 * @returns true if successful, false if failed
 */
bool HEARTRATE::setSamplingRate(uint16_t rate) {
  MAX30102::SamplingRate samplingRateEnum;

  switch (rate) {
    case 50:
      samplingRateEnum = MAX30102::SAMPLING_RATE_50SPS;
      break;
    case 100:
      samplingRateEnum = MAX30102::SAMPLING_RATE_100SPS;
      break;
    case 200:
      samplingRateEnum = MAX30102::SAMPLING_RATE_200SPS;
      break;
    case 400:
      samplingRateEnum = MAX30102::SAMPLING_RATE_400SPS;
      break;
    case 800:
      samplingRateEnum = MAX30102::SAMPLING_RATE_800SPS;
      break;
    case 1000:
      samplingRateEnum = MAX30102::SAMPLING_RATE_1000SPS;
      break;
    case 1600:
      samplingRateEnum = MAX30102::SAMPLING_RATE_1600SPS;
      break;
    case 3200:
      samplingRateEnum = MAX30102::SAMPLING_RATE_3200SPS;
      break;
    default:
      return false;
  }

  if (obj.setSamplingRate(samplingRateEnum)) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}

/**
 * @param samples Number of samples to average (0, 2, 4, 8, 16, 32), Default: 0
 * @returns true if successful, false if failed
 */
bool HEARTRATE::setSampleAveraging(uint8_t samples) {
  MAX30102::SampleAveraging sampleAveragingEnum;

  switch (samples) {
    case 0:
      sampleAveragingEnum = MAX30102::SMP_AVE_NONE;
      break;
    case 2:
      sampleAveragingEnum = MAX30102::SMP_AVE_2;
      break;
    case 4:
      sampleAveragingEnum = MAX30102::SMP_AVE_4;
      break;
    case 8:
      sampleAveragingEnum = MAX30102::SMP_AVE_8;
      break;
    case 16:
      sampleAveragingEnum = MAX30102::SMP_AVE_16;
      break;
    case 32:
      sampleAveragingEnum = MAX30102::SMP_AVE_32;
      break;
    default:
      return false;
  }

  if (obj.setSampleAveraging(sampleAveragingEnum)) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}


/** 
 * @param led "LED_RED" or "LED_IR"
 * @param current Current in mA (0-50) in 0.2mA steps, Default: Red 18mA, IR 16mA
 * @returns true if successful, false if failed
 */
bool HEARTRATE::setLedCurrent(const char *led, float current) {
  MAX30102::Led ledEnum;

  if (strcmp(led, "LED_RED") == 0)
  {
    ledEnum = MAX30102::LED_RED;
  }
  else if (strcmp(led, "LED_IR") == 0)
  {
    ledEnum = MAX30102::LED_IR;
  }
  else
  {
    return false;
  }

  uint8_t current8bit;
  if ((current >= 0) && (current <= 50) && (fmod(current, 0.2) == 0))
  {
    current8bit = current/0.2;
  }
  else
  {
    return false;
  }

  if (obj.setLedCurrent(ledEnum, current8bit))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * @param pulseWidth Pulse width in µs (69, 118, 215, 411) Affects resolution (15bit, 16bit, 17bit, 18bit), Default: 411 µs/18bit
 * @returns true if successful, false if failed
 */
bool HEARTRATE::setPulseWidth(uint16_t pulseWidth) {
  MAX30102::Resolution pulseWidthEnum;

  switch (pulseWidth) {
    case 69:
      pulseWidthEnum = MAX30102::RESOLUTION_15BIT_69US;
      break;
    case 118:
      pulseWidthEnum = MAX30102::RESOLUTION_16BIT_118US;
      break;
    case 215:
      pulseWidthEnum = MAX30102::RESOLUTION_17BIT_215US;
      break;
    case 411:
      pulseWidthEnum = MAX30102::RESOLUTION_18BIT_4110US; // Typo in the library, should be 411US
      break;
    default:
      return false;
  }

  if (obj.setResolution(pulseWidthEnum))
  {
    return true;
  } 
  else 
  {
    return false;
  }
}







void HEARTRATE::loop() {
  auto sample = obj.readSample(1000);
  float current_value_red = sample.red;
  float current_value_ir = sample.ir;

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
