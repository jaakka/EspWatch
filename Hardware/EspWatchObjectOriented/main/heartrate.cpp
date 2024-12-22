#include "MAX30102.h"
#include "heartrate.h"
#include <Arduino.h>
#include "heartratefilters.h"


HEARTRATE::HEARTRATE() : obj(MAX30102()) {}

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
    sensorEnabled = true;
}

void HEARTRATE::disableSensor() {
    sensorEnabled = false;
}

bool HEARTRATE::pulseDetected() {
    return wrist; //THIS IS TEST, IF WRIST DETECTED MAKE PULSE ANIMATIONS
}

void HEARTRATE::loop() {
  auto sample = obj.readSample(1000);
  float current_value = sample.red;
  avgPulse = current_value;

  if(sample.red > kFingerThreshold) {
    if(millis() - finger_timestamp > kFingerCooldownMs) {
      finger_detected = true;
      wrist = true;
      lastWrist = millis();
    }
  }

  if(wrist) 
  {
    if((lastWrist + 1000) < millis())
    {
      wrist = false; 
    }
  }

}

int HEARTRATE::getAvgPulse() {
    return avgPulse;
}

bool HEARTRATE::wristDetected() { //käyttäjän ranne tunnistettu
      return wrist;
}