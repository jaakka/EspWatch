#include "pulseapp.h"

void PulseApp::init() {
  pulseAnim = 0;
}

void PulseApp::handleApplication() {
  handleApplicationAllTime();

  if(touch.userSwipeUp()) {
    exit_application = true;
  }
}

void PulseApp::handleApplicationAllTime() {
  // If menu or pulseapp running
  if(runningApp == 0 || runningApp == 2) {
    pulseAnimation();
  }
}

void PulseApp::pulseAnimation() {
  if(pulseAnim > 0){
    pulseAnim--;
  } 
  else {
    pulseAnim = 10; 
  }
}

void PulseApp::handleApplicationBackground() {
  handleApplicationAllTime();
}

void PulseApp::drawApplication(int x, int y, float scale) {
  const float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  const float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  const float abs_middle_x = x + scaled_width/2;
  const float abs_middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width/2 + 2, rgb(50, 50, 50)); // +2 fixs ghost pixels
  float heart_radius = (scaled_width/2)/6;

  heart_radius = (heart_radius*0.8) + (heart_radius*0.2)*(pulseAnim/10);
  drawHeartIcon(abs_middle_x, abs_middle_y - heart_radius/2, heart_radius);
  float font_scale = (scale/10)*2;
  frame.setTextColor(rgb(255,255,255));
  frame.setTextSize(font_scale);
  frame.drawCentreString("60 bpm", abs_middle_x, abs_middle_y + scale*3, 2);
}

void PulseApp::drawApplicationIcon(int x, int y, float scale) {
  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;
  frame.fillCircle(x, y, scaled_width/2, rgb(50,50,50));

  float heart_radius = (scaled_width/2)/4;

  heart_radius = heart_radius + (heart_radius*0.2)*(pulseAnim/10);
  drawHeartIcon(x, y, heart_radius);
}

void PulseApp::drawHeartIcon(float x, float y, float heart_scale) {
  // Draw scaleable heart icon
  float left_circle_x = x - (heart_scale/1.8)*0.95;
  float left_circle_y = y - heart_scale/1.8;
  float left_circle_x_angled, left_circle_y_angled;
  getPositionByAngle(left_circle_x, left_circle_y, 135, heart_scale, left_circle_x_angled, left_circle_y_angled);
  frame.fillCircle(left_circle_x, left_circle_y, heart_scale, rgb(255,0,0)); 

  float right_circle_x = x + (heart_scale/1.8)*0.95;
  float right_circle_y = y - heart_scale/1.8;
  float right_circle_x_angled, right_circle_y_angled;
  getPositionByAngle(right_circle_x, right_circle_y, 45, heart_scale, right_circle_x_angled, right_circle_y_angled);
  frame.fillCircle(right_circle_x, right_circle_y, heart_scale, rgb(255,0,0)); 
  
  frame.fillTriangle( 
    left_circle_x_angled, //Left
    left_circle_y_angled, 
    right_circle_x_angled, //Right
    right_circle_y_angled, 
    x, 
    y + (heart_scale * 1.5), //Bottom
    rgb(255,0,0)
  );
}

bool PulseApp::areAvailableOnMenu() {
  return true;
}
