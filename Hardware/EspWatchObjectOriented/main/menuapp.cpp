#include "menuapp.h"
#include <TFT_eSPI.h>

extern TFT_eSPI lcd;

void MenuApp::init() {

}

void MenuApp::handleApplication() {

}

void MenuApp::handleApplicationBackground() {

}

void MenuApp::drawApplication(float scale) {

}

void MenuApp::drawApplicationIcon(float scale) {
  
}

bool MenuApp::areAvailableOnMenu() {
  return false;
}

void MenuApp::smoothMove(int* current_x, int* current_y, int target_x, int target_y) {
    float ratio, xspeed, yspeed, speed = 5;
    float xdistance = abs(target_xpos - current_x); 
    float ydistance = abs(target_ypos - current_y);

    if (xdistance > ydistance) {
        ratio = (float)ydistance / xdistance;
        yspeed = (int)(speed * ratio);
        xspeed = speed;
    } else if (ydistance > xdistance) {
        ratio = (float)xdistance / ydistance;
        xspeed = (int)(speed * ratio);
        yspeed = speed;
    } else {
        xspeed = speed;
        yspeed = speed;
    }

    for (int i = 0; i < speed; i++) {
        if (current_x != target_xpos) {
            if (current_x > target_xpos) {
                current_x -= xspeed;
                if (current_x < target_xpos) current_x = target_xpos;
            } else if (current_x < target_xpos) {
                current_x += xspeed;
                if (current_x > target_xpos) current_x = target_xpos;
            }
        }
        if (current_y != target_ypos) {
            if (current_y > target_ypos) {
                current_y -= yspeed;
                if (current_y < target_ypos) current_y = target_ypos;
            } else if (current_y < target_ypos) {
                current_y += yspeed;
                if (current_y > target_ypos) current_y = target_ypos;
            }
        }
    }
}