#include "menuapp.h"

void MenuApp::init() {

}

void MenuApp::handleApplication() {

}

void MenuApp::handleApplicationBackground() {

}

int MenuApp::checkApplicationId(int id) {
  if(id < APP_COUNT) {
    if(apps[id]->areAvailableOnMenu()) {
      return id;
    } 
    else {
      return -1; // Application not available on menu
    }
  }
  else {
    return -1; // Application not exists
  }
}

void MenuApp::drawApplication(int x, int y, float scale) {

  float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  float middle_x = x + scaled_width/2;
  float middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(middle_x, middle_y, scaled_width/2, rgb(150, 0, 0));

  //Middle application
  apps[1]->drawApplicationIcon(middle_x,middle_y); // 0 is menu app, 1 is clock app hardcoded.

  //Center circle applications
  for (int i = 0; i < 6; i++) {
    float angle = ((2 * PI * (i-1)) / 6) + 0.7853981634; // 45 c
    int radius = 75;
    float x = middle_x + radius * cos(angle);
    float y = middle_y + radius * sin(angle);
    int applicationId = i + 2;

    float scaled_width = ((float)SCREEN_WIDTH/10) * 2.8;

    if(checkApplicationId(applicationId) != -1) {
      apps[applicationId]->drawApplicationIcon(x,y);
    } 
    else {
      frame.fillCircle(x, y, scaled_width/2, rgb(80,0,0));
    }
  }

  //Out circle applications
  for (int i = 0; i < 10; i++) {
    float angle = ((2 * PI * (i-1)) / 10);
    int radius = 145;
    float x = middle_x + radius * cos(angle);
    float y = middle_y + radius * sin(angle);
    int applicationId = i + 2;

    float scaled_width = ((float)SCREEN_WIDTH/10) * 2.8;

    if(checkApplicationId(applicationId) != -1) {
      apps[applicationId]->drawApplicationIcon(x,y);
    } 
    else {
      frame.fillCircle(x, y, scaled_width/2, rgb(80,0,0));
    }
  }
}

void MenuApp::smoothMove(int* current_x, int* current_y, int target_x, int target_y) {
    float ratio, xspeed, yspeed, speed = 5;
    /*float xdistance = abs(target_x - current_x); 
    float ydistance = abs(target_y - current_y);

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
              if (current_x < target_xpos) {
                current_x = target_xpos;
              }
            }
            else if (current_x < target_xpos) {
              current_x += xspeed;
              if (current_x > target_xpos) {
                current_x = target_xpos;
              }
            }
            
        }

        if (current_y != target_ypos) {

            if (current_y > target_ypos) {
                current_y -= yspeed;
                if (current_y < target_ypos) {
                  current_y = target_ypos;
                }
            } else if (current_y < target_ypos) {
                current_y += yspeed;
                if (current_y > target_ypos) {
                  current_y = target_ypos;
                }
            }

        }
    }*/
}

void MenuApp::drawApplicationIcon(int x, int y, float scale) {
  // Application not need icon
}

bool MenuApp::areAvailableOnMenu() {
  return false;
}