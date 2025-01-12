#include "pulseapp.h"

void PulseApp::init() {

}

void PulseApp::handleApplication() {

}

void PulseApp::handleApplicationBackground() {

}

void PulseApp::drawApplication(int x, int y, float scale) {

}

void PulseApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  frame.fillCircle(x, y, scaled_width/2, rgb(100,100,100));
}

bool PulseApp::areAvailableOnMenu() {
  return true;
}