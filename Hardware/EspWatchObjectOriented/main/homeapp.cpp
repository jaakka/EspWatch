#include "homeapp.h"
#include <TFT_eSPI.h>

void HomeApp::init() {

}

void HomeApp::handleApplication() {

}

void HomeApp::handleApplicationBackground() {

}

void HomeApp::drawApplication(int x, int y, float scale) {
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

void HomeApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  drawApplication(x - scaled_width/2 ,y - scaled_height/2 ,scale);
}

bool HomeApp::areAvailableOnMenu() {
  return true;
}