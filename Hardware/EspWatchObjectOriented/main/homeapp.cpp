#include "homeapp.h"
#include <TFT_eSPI.h>

void HomeApp::init() {
  last_release = millis();
  edit_mode = false;
  analog_watch = true;
}

void HomeApp::handleApplication() {
  if (!edit_mode) {
    if(touch.userSwipeUp()) {
      exit_application = true;
    }
    if(touch.userSwipeRight() && analog_watch) {
      analog_watch = false;
    }
    if(touch.userSwipeLeft() && !analog_watch) {
      analog_watch = true;
    }
  }


  if(touch.userTouch()) {
    if (millis() - last_release > 1000) {
      float center_x = SCREEN_WIDTH / 2;
      float center_y = SCREEN_HEIGHT / 2;
      float current_angle = atan2(touch.y - center_y, touch.x - center_x);
      if (!edit_mode) {
        edit_mode = true;
        last_angle = current_angle;
      } else {
        // Calculate angle difference
        float angle_diff = current_angle - last_angle;
        
        // Normalize angle to -PI to PI
        if (angle_diff > PI) angle_diff -= 2 * PI;
        if (angle_diff < -PI) angle_diff += 2 * PI;
        
        // Convert to minutes (60 minutes = 2PI)
        int minute_diff = (angle_diff / (2 * PI)) * 60;
        
        if (abs(minute_diff) > 0) {
          // Update time
          time_t t = now();
          t += minute_diff * 60; // Convert to seconds
          setTime(t);
          last_angle = current_angle;
        }
      }
    }
  }
  else {
    // User released touch
    last_release = millis();
    edit_mode = false;
  }
}

void HomeApp::handleApplicationBackground() {

}

void HomeApp::drawApplication(int x, int y, float scale) {
  if (analog_watch) {
    drawAnalogWatch(x, y, scale);
  }
  if (!analog_watch) {
    drawDigitalWatch(x, y, scale);
  }
}

void HomeApp::drawAnalogWatch(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  float middle_x = x + scaled_width/2;
  float middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(middle_x, middle_y, scaled_width/2 + 2, rgb(255, 0, 0)); // +2 fixs ghost pixels

  // Draw side lines
  for(int i = 0; i < 12; i++) {
    float angle = i * 30;
    float x1 = cos(angle * PI / 180) * (scaled_width/2) + middle_x;
    float y1 = sin(angle * PI / 180) * (scaled_height/2) + middle_y;
    float x2 = cos(angle * PI / 180) * ((scaled_width/2) - 8) + middle_x;
    float y2 = sin(angle * PI / 180) * ((scaled_height/2) - 8) + middle_y;

    for (int offset = -1; offset <= 1; offset++) {
      frame.drawLine(x1 + offset, y1, x2 + offset, y2, rgb(0, 0, 0));
      frame.drawLine(x1, y1 + offset, x2, y2 + offset, rgb(0, 0, 0));
    }
  }

  // Draw hour line
  uint16_t HourColor = rgb(0, 0, 0);
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; 
  float hourX = cos(hourAngle) * ((scaled_width/2)*0.6) + middle_x;
  float hourY = sin(hourAngle) * ((scaled_height/2)*0.6) + middle_y; 

  for (int offset = -2; offset <= 2; offset++) { 
    frame.drawLine(middle_x + offset, middle_y, hourX + offset, hourY, HourColor);
    frame.drawLine(middle_x, middle_y + offset, hourX, hourY + offset, HourColor);
  }

  // Draw large middle dot
  float middle_dot_size = 7;
  frame.fillCircle(middle_x, middle_y, (middle_dot_size/10) * scale , HourColor);

  // Draw minute line
  uint16_t MinuteColor = rgb(100, 100, 100);

  if (edit_mode) {
    MinuteColor = rgb(0, 255, 0);
  }

  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * ((scaled_width/2)*0.8) + middle_x;
  float minuteY = sin(minuteAngle) * ((scaled_height/2)*0.8) + middle_y; 

  for (int offset = -1; offset <= 1; offset++) { 
    frame.drawLine(middle_x + offset, middle_y, minuteX + offset, minuteY, MinuteColor);
    frame.drawLine(middle_x, middle_y + offset, minuteX, minuteY + offset, MinuteColor);
  }

  // Draw middle dot
  middle_dot_size = 5;
  frame.fillCircle(middle_x, middle_y, (middle_dot_size/10) * scale , MinuteColor);

  // Draw second line
  uint16_t SecondColor = rgb(200, 200, 200);
  float secondAngle = (second() * 6) * PI / 180 - PI / 2;
  float secondX = cos(secondAngle) * ((scaled_width/2)*0.8) + middle_x;
  float secondY = sin(secondAngle) * ((scaled_height/2)*0.8) + middle_y; 
  frame.drawLine(middle_x, middle_y, secondX, secondY, SecondColor);

  // Draw small middle dot
  middle_dot_size = 3;
  frame.fillCircle(middle_x, middle_y, (middle_dot_size/10) * scale , SecondColor);
}

void HomeApp::drawDigitalWatch(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  float middle_x = x + scaled_width/2;
  float middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(middle_x, middle_y, scaled_width/2 + 2, rgb(255, 0, 0)); // +2 fixs ghost pixels
  
  frame.setTextColor(rgb(255,255,255));

  if (edit_mode) {
    frame.setTextColor(rgb(0,255,0));
  }

  float font_scale = (scale/10) * 3;
  frame.setTextSize(font_scale);
  frame.drawCentreString(digitalTime(), middle_x, middle_y - font_scale*8, 4);
}

String HomeApp::digitalTime() {
  return addZerosIfNeeded(hour()) + ":" + addZerosIfNeeded(minute());
}

String HomeApp::addZerosIfNeeded(int value) {
  if(value > 9) {
    return String(value);
  }else{
    return "0"+String(value);
  }
}

void HomeApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  drawApplication(x - scaled_width/2 ,y - scaled_height/2 ,scale);
}

bool HomeApp::areAvailableOnMenu() {
  return true;
}