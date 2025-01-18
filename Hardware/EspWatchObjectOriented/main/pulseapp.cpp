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
  frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width/2 + 2, rgb(100, 100, 100)); // +2 fixs ghost pixels
}

void PulseApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  frame.fillCircle(x, y, scaled_width/2, rgb(100,100,100));
}

bool PulseApp::areAvailableOnMenu() {
  return true;
}