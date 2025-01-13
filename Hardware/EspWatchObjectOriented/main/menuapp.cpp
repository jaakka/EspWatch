#include "menuapp.h"

void MenuApp::init() {
  // Init values
  menu_xpos = 0;
  menu_ypos = 0;
  target_xpos = 0;
  target_ypos = 0;
  isDragging = false;
  last_touch = millis(); 
  last_release = millis();
  near_app_x = 0;
  near_app_y = 0;
  touch_start_x = 0;
  touch_start_y = 0;
  openApplicationScale = 0;
}

void MenuApp::handleApplication() {
  if (touch.userTouch() && openApplicationScale < 1.5) {
    last_touch = millis();
    if (!isDragging) {
      touch_start_x = touch.x;
      touch_start_y = touch.y;
      isDragging = true;
    } else {
      // Relative movement
      menu_xpos += (touch.x - touch_start_x);
      menu_ypos += (touch.y - touch_start_y);
      touch_start_x = touch.x;
      touch_start_y = touch.y;
      Serial.println("mouseX: "+String(menu_xpos)+", mouseY:"+String(menu_ypos));
    }
  } else {
    //User has released the touch
    if(isDragging)
    {
     /* if(last_release + 1000 > millis() && abs(menu_x-selectPosX)<5 && abs(menu_y-selectPosY)<5 && (selectedApplication == 4 || selectedApplication == 5))
      {
        Serial.println("start app");
        openApp = true;
      }*/
      last_release = millis();
      //selectPosX = menu_x, selectPosY = menu_ypos;
      getNearestApplication(&near_app_x, &near_app_y, x_offset, y_offset, 2.8);
    }
    isDragging = false;
  }

  if (!isDragging && last_touch + 100 < millis()) { //SmoothAjo6000
    smoothMove(&menu_xpos,&menu_ypos,near_app_x,near_app_y);
  }
}

void MenuApp::handleApplicationBackground() {
  //Menu not need background activity
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

  middle_y = menu_ypos + y + scaled_height/2;
  middle_x = menu_xpos + x + scaled_width/2;

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

void MenuApp::getNearestApplication(float *near_app_x, float *near_app_y, float x_offset, float y_offset, float scale) {

}

void MenuApp::smoothMove(float *current_x, float *current_y, float target_x, float target_y) {
    float ratio, xspeed, yspeed, speed = 5;
    float xdistance = abs((target_x - *current_x)); 
    float ydistance = abs((target_y - *current_y));

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

        if (*current_x != target_xpos) {

            if (*current_x > target_xpos) {
              *current_x -= xspeed;
              if (*current_x < target_xpos) {
                *current_x = target_xpos;
              }
            }
            else if (*current_x < target_xpos) {
              *current_x += xspeed;
              if (*current_x > target_xpos) {
                *current_x = target_xpos;
              }
            }
            
        }

        if (*current_y != target_ypos) {

            if (*current_y > target_ypos) {
                *current_y -= yspeed;
                if (*current_y < target_ypos) {
                  *current_y = target_ypos;
                }
            } else if (*current_y < target_ypos) {
                *current_y += yspeed;
                if (*current_y > target_ypos) {
                  *current_y = target_ypos;
                }
            }

        }
    }
}

void MenuApp::drawApplicationIcon(int x, int y, float scale) {
  // Application not need icon
}

bool MenuApp::areAvailableOnMenu() {
  return false;
}