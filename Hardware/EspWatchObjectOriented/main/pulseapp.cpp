#include "pulseapp.h"

void PulseApp::init() {

}

void PulseApp::handleApplication() {
  if(touch.userSwipeUp()) {
    exit_application = true;
  }
}

void PulseApp::handleApplicationBackground() {

}

void PulseApp::drawApplication(int x, int y, float scale) {
  const float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  const float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  const float abs_middle_x = x + scaled_width/2;
  const float abs_middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width/2 + 2, rgb(50, 50, 50)); // +2 fixs ghost pixels
}

void PulseApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  frame.fillCircle(x, y, scaled_width/2, rgb(50,50,50));

  float heart_radius = (scaled_width/2)/4;

  // Draw scaleable heart icon
  frame.fillCircle(x - heart_radius/1.8  , y - heart_radius/1.8 , heart_radius, rgb(255,0,0)); 
  frame.fillCircle(x + heart_radius/1.8 , y - heart_radius/1.8 , heart_radius, rgb(255,0,0)); 
  frame.fillTriangle( 
    x - (heart_radius/2 + heart_radius), //Left
    y - heart_radius/1.8, 
    x + (heart_radius/2 + heart_radius), //Right
    y - heart_radius/1.8, 
    x, 
    y + (heart_radius/2 + heart_radius), //Bottom
    rgb(255,0,0)
    );
}

bool PulseApp::areAvailableOnMenu() {
  return true;
}