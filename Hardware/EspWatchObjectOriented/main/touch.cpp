#include "touch.h"
#include <CST816S.h>
#include <Arduino.h>

#define TOUCH_SDA 21
#define TOUCH_SCL 22
#define TOUCH_RST 4
#define TOUCH_INT 19

TOUCH::TOUCH() : obj(CST816S(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT)) {}

void TOUCH::init() {
  x=0; y=0;
  Serial.println("TOUCH init");
  obj.begin();
}

void TOUCH::loop() {
  if(obj.available()) {
    touchData.available = true;
    touchData.gestureID = obj.data.gestureID;
    x = obj.data.y;
    y = 240-obj.data.x;
  }
  else 
  {
    touchData.available = false;
  }
}

bool TOUCH::userSwipeRight() {
  if(touchData.available && touchData.gestureID == SWIPE_UP) { //SWIPE_RIGHT
    return true;
  }
  return false;
}

bool TOUCH::userSwipeLeft() {
  if(touchData.available && touchData.gestureID == SWIPE_DOWN) {
    return true;
  }
  return false;
}

bool TOUCH::userSwipeUp() {
  if(touchData.available && touchData.gestureID == SWIPE_RIGHT) {
    return true;
  }
  return false;
}

bool TOUCH::userSwipeDown() {
  if(touchData.available && touchData.gestureID == SWIPE_LEFT) {
    return true;
  }
  return false;
}

bool TOUCH::userTouch() {
  if(touchData.available) {
    return true;
  }
  return false;
}