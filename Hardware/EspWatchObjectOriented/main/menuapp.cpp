#include "menuapp.h"

void MenuApp::init() {
  // Init values
  draw_x = 0;
  draw_y = 0;
  draw_scale = 0;
  menu_xpos = 0;
  menu_ypos = 0;
  target_xpos = 0;
  target_ypos = 0;
  is_dragging = false;
  last_touch = millis(); 
  last_release = millis();
  near_app_x = 0;
  near_app_y = 0;
  near_app_id = 0;
  touch_start_x = 0;
  touch_start_y = 0;
  application_open_scale = 0;
}

void MenuApp::handleApplication() {
  if (touch.userTouch() && application_open_scale < 1.5) {
    last_touch = millis();
    if (!is_dragging) {
      touch_start_x = touch.x;
      touch_start_y = touch.y;
      is_dragging = true;
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
    if(is_dragging)
    {
     /* if(last_release + 1000 > millis() && abs(menu_x-selectPosX)<5 && abs(menu_y-selectPosY)<5 && (selectedApplication == 4 || selectedApplication == 5))
      {
        Serial.println("start app");
        openApp = true;
      }*/
      last_release = millis();
      //getNearestApplication(menu_xpos, menu_ypos, near_app_x, near_app_y, near_app_id);
    }
    is_dragging = false;
  }

  if (!is_dragging && last_touch + 100 < millis()) { //SmoothAjo6000
    smooth_move(menu_xpos,menu_ypos,near_app_x,near_app_y);
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

void MenuApp::drawApplication(int x, int y, float scale) { //TODO: DRAW SELECTED APP LAST
  draw_x = x;
  draw_y = y;
  draw_scale = scale;

  const float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  const float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  const float abs_middle_x = x + scaled_width/2;
  const float abs_middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width/2, rgb(150, 0, 0));

  const float middle_y = menu_ypos + y + scaled_height/2;
  const float middle_x = menu_xpos + x + scaled_width/2;

  const float application_scaled_width = ((float)SCREEN_WIDTH/10) * 2.8;
  int application_id_var = 0;

  //Middle application
  //frame.fillCircle(middle_x, middle_y, application_scaled_width/2 + (scale*0.2), rgb(255,255,255));
  application_id_var++;

  float shortest_distance = distance(x + abs_middle_x, y + abs_middle_y, middle_x, middle_y);
  int cur_app_id = application_id_var;
  float cur_app_x = x;
  float cur_app_y = y;

  float appscale = 3.5 - (shortest_distance/150)*1.3;
  apps[application_id_var]->drawApplicationIcon(middle_x,middle_y,appscale); // 0 is menu app, 1 is clock app hardcoded.

  frame.setTextColor(rgb(255,255,255));
  frame.drawString(String(application_id_var), middle_x, middle_y, 2);

  //Center circle applications
  for (int i = 0; i < 6; i++) {
    const float angle = ((2 * PI * (i-1)) / 6);// + 0.7853981634; // 45 c
    const int radius = 80;
    const float app_x = middle_x + radius * cos(angle);
    const float app_y = middle_y + radius * sin(angle);
    
    application_id_var++;
    
    float dist = distance(x + abs_middle_x ,y + abs_middle_y, app_x, app_y);
    if (shortest_distance > dist)
    {
      shortest_distance = dist;
      cur_app_id = application_id_var;
      //cur_app_x = app_x;
      //cur_app_y = app_y;
    }

    if(checkApplicationId(application_id_var) != -1) {
        appscale = 3.5 - (dist/150)*1.3;
        apps[application_id_var]->drawApplicationIcon(app_x,app_y,appscale);
    } 
    else 
    {
      appscale = 1.2 - (dist/150)*0.6;
      frame.fillCircle(app_x, app_y, (application_scaled_width/2)*appscale, rgb(120,0,0));
    }

    frame.setTextColor(rgb(255,255,255));
    frame.drawString(String(application_id_var), app_x, app_y, 2);
  }

  //Out circle applications
  for (int i = 0; i < 11; i++) {
    const float angle = ((2 * PI * (i-1)) / 11);
    const int radius = 152;
    const float app_x = middle_x + radius * cos(angle);
    const float app_y = middle_y + radius * sin(angle);

    const float scaled_width = ((float)SCREEN_WIDTH/10); //* 2.8;
    application_id_var++;

    float dist = distance(x + abs_middle_x ,y + abs_middle_y, app_x, app_y);
    if (shortest_distance > dist)
    {
      shortest_distance = dist;
      cur_app_id = application_id_var;
     // near_app_x = x;
      //near_app_y = y;
    }

    if(checkApplicationId(application_id_var) != -1) {
        appscale = 3.5 - (dist/150)*1.3;
        apps[application_id_var]->drawApplicationIcon(app_x,app_y,appscale);
    } 
    else 
    {
      appscale = 1.2 - (dist/150)*0.6;
      frame.fillCircle(app_x, app_y, (application_scaled_width/2)*appscale, rgb(120,0,0));
    }

    frame.setTextColor(rgb(255,255,255));
    frame.drawString(String(application_id_var), app_x, app_y, 2);
  }

  // Draw cursor
  if (is_dragging) {
    frame.fillCircle(abs_middle_x, abs_middle_y, scale, rgb(255, 255, 255));
  }

  frame.fillCircle(near_app_x, near_app_y, scale, rgb(0, 255, 0));

  Serial.println(shortest_distance);
  near_app_id = cur_app_id;
  near_app_x = cur_app_x;
  near_app_y = cur_app_y;
}

void MenuApp::drawApplicationIcon(int x, int y, float scale) {
  // Application not need icon
}

bool MenuApp::areAvailableOnMenu() {
  return false;
}