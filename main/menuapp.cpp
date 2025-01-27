#include "menuapp.h"

void MenuApp::init() {
  // Init values
  menu_xpos = 0;
  menu_ypos = 0;
  is_dragging = false;
  last_touch = millis(); 
  last_release = millis();
  near_app_x = 0;
  near_app_y = 0;
  near_app_id = 0;
  touch_start_x = 0;
  touch_start_y = 0;
  application_open_scale = 10;//4;
  debug_mode = false;
  enable_cursor = false;
  application_starting = false;
  application_closing = false;
}

void MenuApp::handleApplication() {
  if(!application_closing && !application_starting) {
    if (touch.userTouch()) {
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
      }
    } 
    else 
    {
      //User has released the touch
      if(is_dragging)
      {

        if(last_release + 1000 > millis() && distance(near_app_x, near_app_y, menu_xpos, menu_ypos) < 10)
        {
          Serial.println("start app");
          application_starting = true;
        }
        last_release = millis();
        Serial.println("Nearest app "+String(near_app_id));
      }
      is_dragging = false;
    }

    if (!is_dragging && last_touch + 100 < millis()) { //SmoothAjo6000
      float target_x = SCREEN_WIDTH / 2 - near_app_x;
      float target_y = SCREEN_HEIGHT / 2 - near_app_y;
      smooth_move(menu_xpos,menu_ypos,near_app_x,near_app_y);
    }
  }
  if (exit_application) {
    exit_application = false;
    applicationClosed();
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

void MenuApp::applicationClosed() {
  application_starting = false;
  application_closing = true;
}

void MenuApp::drawApplication(int x, int y, float scale) { //TODO: DRAW SELECTED APP LAST
  const float scaled_width = ((float)SCREEN_WIDTH/10) * scale;
  const float scaled_height = ((float)SCREEN_HEIGHT/10) * scale;

  const float abs_middle_x = x + scaled_width/2;
  const float abs_middle_y = y + scaled_height/2;

  // Draw background
  frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width/2 + 2, rgb(150, 0, 0)); // +2 fixs ghost pixels

  const float middle_y = menu_ypos + y + scaled_height/2;
  const float middle_x = menu_xpos + x + scaled_width/2;

  const float application_scaled_width = ((float)SCREEN_WIDTH/10) * 2.8;
  int application_id_var = 0;

  //Middle application
  //frame.fillCircle(middle_x, middle_y, application_scaled_width/2 + (scale*0.2), rgb(255,255,255));
  application_id_var++;

  float shortest_distance = distance(x + abs_middle_x, y + abs_middle_y, middle_x, middle_y);
  int cur_app_id = application_id_var;
  float cur_app_x = x + middle_x;
  float cur_app_y = y + middle_y;

  float center_appscale = 3.5 - (shortest_distance/150)*1.3;
  float appscale = center_appscale;
  apps[application_id_var]->drawApplicationIcon(middle_x,middle_y,center_appscale); // 0 is menu app, 1 is clock app hardcoded.
  if (debug_mode) {
    frame.setTextColor(rgb(255,255,255));
    frame.drawString(String(application_id_var), middle_x, middle_y, 2);
  }
  float largest_center_app = 0;
  //Center circle applications
  for (int i = 0; i < 6; i++) {
    const float angle = ((2 * PI * (i-1)) / 6);// + 0.7853981634; // 45 c
    const int radius = 83 + center_appscale;
    const float app_x = middle_x + radius * cos(angle);
    const float app_y = middle_y + radius * sin(angle);
    
    application_id_var++;
    
    float dist = distance(x + abs_middle_x ,y + abs_middle_y, app_x, app_y);
    bool application_available = checkApplicationId(application_id_var) != -1;
    if (shortest_distance > dist && application_available)
    {
      shortest_distance = dist;
      cur_app_id = application_id_var;
      cur_app_x = app_x;
      cur_app_y = app_y;
    }

    if(application_available) {
        appscale = 3.5 - (dist/150)*1.3;
        apps[application_id_var]->drawApplicationIcon(app_x,app_y,appscale);
    } 

    if(!application_available) 
    {
      appscale = 1.3 - (dist/150)*0.6;
      if (debug_mode) {
        frame.fillCircle(app_x, app_y, (application_scaled_width/2)*appscale, rgb(120,0,0));
      }
    }
    if(largest_center_app<appscale) {largest_center_app=appscale;}
    if (debug_mode) {
      frame.setTextColor(rgb(255,255,255));
      frame.drawString(String(application_id_var), app_x, app_y, 2);
    }
  }

  //Out circle applications
  for (int i = 0; i < 11; i++) {
    const float angle = ((2 * PI * (i-1)) / 11);
    const int radius = 160 + center_appscale + largest_center_app;
    const float app_x = middle_x + radius * cos(angle);
    const float app_y = middle_y + radius * sin(angle);

    const float scaled_width = ((float)SCREEN_WIDTH/10); //* 2.8;
    application_id_var++;

    float dist = distance(x + abs_middle_x ,y + abs_middle_y, app_x, app_y);
    bool application_available = checkApplicationId(application_id_var) != -1;
    if (shortest_distance > dist && application_available)
    {
      shortest_distance = dist;
      cur_app_id = application_id_var;
      cur_app_x = app_x;
      cur_app_y = app_y;
    }

    if(application_available) {
        appscale = 3.5 - (dist/150)*1.3;
        apps[application_id_var]->drawApplicationIcon(app_x,app_y,appscale);
    } 
    
    if(!application_available)
    {
      appscale = 1.3 - (dist/150)*0.6;
      if(debug_mode) {
        frame.fillCircle(app_x, app_y, (application_scaled_width/2)*appscale, rgb(120,0,0));
      }
    }

    if (debug_mode) {
      frame.setTextColor(rgb(255,255,255));
      frame.drawString(String(application_id_var), app_x, app_y, 2);
    }
  }

  // Draw cursor
  if (is_dragging && enable_cursor) {
    frame.fillCircle(abs_middle_x, abs_middle_y, scale, rgb(255, 255, 255));
  }

  near_app_id = cur_app_id;
  near_app_x = menu_xpos + (SCREEN_WIDTH / 2 - cur_app_x);
  near_app_y = menu_ypos + (SCREEN_HEIGHT / 2 - cur_app_y);


  if(application_starting) {
    if(application_open_scale < 10) {
      application_open_scale+=1;
    }else{runningApp=near_app_id;}
    float center_x = SCREEN_WIDTH/2 - ((SCREEN_WIDTH/2)/10)*application_open_scale;
    float center_y = SCREEN_HEIGHT/2 - ((SCREEN_HEIGHT/2)/10)*application_open_scale;
    apps[near_app_id]->drawApplication(center_x, center_y, application_open_scale);
  }

  if(application_closing) {
    if(application_open_scale > 4) {
      application_open_scale-=1;
    }else{application_closing = false;}
    float center_x = SCREEN_WIDTH/2 - ((SCREEN_WIDTH/2)/10)*application_open_scale;
    float center_y = SCREEN_HEIGHT/2 - ((SCREEN_HEIGHT/2)/10)*application_open_scale;
    apps[near_app_id]->drawApplication(center_x, center_y, application_open_scale);
  }
}

void MenuApp::drawApplicationIcon(int x, int y, float scale) {
  // Application not need icon
}

bool MenuApp::areAvailableOnMenu() {
  return false;
}
