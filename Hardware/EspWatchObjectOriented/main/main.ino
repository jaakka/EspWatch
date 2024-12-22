#include <TFT_eSPI.h>
#include "touch.h"
#include <TimeLib.h> // (Time by Michael Margolis).

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define SCREEN_CALCULATED 57600 // 240 * 240

TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite frame = TFT_eSprite(&lcd);
TOUCH touch;

uint16_t menuColors[] = {
  0x8410,
  0x8410,
  0x03BF, // Harmaa
  0x8410, // Harmaa
  0x8410, // Harmaa
  0x8410, // Harmaa
  0x8410, // Harmaa
  0x8410, // Harmaa
  0x8410, // Harmaa
  0x8410
};

enum App
{
  HOME = 0,
  MENU = 1,
  SETTINGS = 2
};

bool openApp = true;
int scaleAnim = 15;
int selectPosX = -1, selectPosY = -1;
int menu_x = 120, menu_y = -50, next_app = -1;
App application = HOME;
int selectedApplication = 2;
int near_app_x = 120, near_app_y = -50; 
bool userWake = false;
int touchStartX = 0;
int touchStartY = 0;
bool isDragging = false;
unsigned long last_touch = 0;
unsigned long last_release = 0;

void setup() {
  Serial.begin(115200);
  touch.init();
  lcd.begin();
  lcd.setRotation(0);
  lcd.fillScreen(0);
  frame.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT, TFT_TRANSPARENT);
  last_release = millis(); 
  setTime(13, 15, 00, 1, 12, 2024);
}

void loop() {
  //if(openApp && scaleAnim < 15 && userWake){scaleAnim++;}else{if(application == MENU){application = HOME;}}

  if(openApp) {
    if(scaleAnim < 15) {
      scaleAnim++;
    }else{
      if(application == MENU)
      {
        if(selectedApplication == 2)
        {
          application = HOME;
        }
      }
    }
  }
  else
  {
    if(scaleAnim > 1)
    {
      scaleAnim--;
    }
  }

  touch.loop();
  if(application == HOME)
  {
    //handleHomeApplication();
    drawApplicationHome();
    handleApplicationHomeTouch();
  }
  else if(application == MENU)
  {
    int x_offset = 5, y_offset = 10, app_size = 60;
    handleApplicationMenuTouch(x_offset, y_offset, app_size);
    drawApplicationMenu(x_offset, y_offset, app_size);
  }
  frame.pushSprite(0,0);
}

uint8_t rgbTo8Bit(uint8_t red, uint8_t green, uint8_t blue) {
  red = red & 0x07;   
  green = green & 0x07; 
  blue = blue & 0x03;  
  return (red << 5) | (green << 2) | blue;
}

void handleApplicationHomeTouch() {
  if(touch.userSwipeRight()) {
    application = MENU;
    openApp = false;
    Serial.println("close app");
  }
}

void drawApplicationHome() {
  //frame.fillSmoothCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, 0x0210, 0x0210);
  frame.fillSmoothCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, 0x03BF, 0x03BF);
  //tässä kohtaa pitäisi piirtää kehälle 12 viivaa niinkuin analogisessa kellossa vaalean kehän sisään
  for(int i = 0; i < 12; i++) {
  float angle = i * 30;
  float x1 = cos(angle * PI / 180) * (SCREEN_WIDTH/2 ) + SCREEN_WIDTH/2;
  float y1 = sin(angle * PI / 180) * (SCREEN_HEIGHT/2 ) + SCREEN_WIDTH/2;
  float x2 = cos(angle * PI / 180) * (SCREEN_WIDTH/2 - 8) + SCREEN_WIDTH/2;
  float y2 = sin(angle * PI / 180) * (SCREEN_HEIGHT/2 - 8) + SCREEN_WIDTH/2;

  // Piirretään paksumpia viivoja
  for (int offset = -1; offset <= 1; offset++) {
    frame.drawLine(x1 + offset, y1, x2 + offset, y2, 0x00);
    frame.drawLine(x1, y1 + offset, x2, y2 + offset, 0x00);
  }
  }
  
  
  // Piirretään tuntiviisari
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; // Tuntiviisari liikkuu myös minuuttien mukaan
  float hourX = cos(hourAngle) * (SCREEN_WIDTH/2 - 38) + SCREEN_WIDTH/2; // Lyhyempi
  float hourY = sin(hourAngle) * (SCREEN_HEIGHT/2 - 38) + SCREEN_WIDTH/2; // Lyhyempi
  for (int offset = -2; offset <= 2; offset++) { // Paksumpi
    frame.drawLine(SCREEN_WIDTH/2 + offset, SCREEN_HEIGHT/2, hourX + offset, hourY, 0x0000);
    frame.drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + offset, hourX, hourY + offset, 0x0000);
  }

  // Piirretään minuuttiviisari
  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * (SCREEN_WIDTH/2 - 24) + SCREEN_WIDTH/2; // Lyhyempi kuin sekuntiviisari
  float minuteY = sin(minuteAngle) * (SCREEN_HEIGHT/2 - 24) + SCREEN_WIDTH/2; // Lyhyempi kuin sekuntiviisari
  for (int offset = -1; offset <= 1; offset++) { // Paksumpi kuin sekuntiviisari
    frame.drawLine(SCREEN_WIDTH/2 + offset, SCREEN_HEIGHT/2, minuteX + offset, minuteY, 0x0000);
    frame.drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + offset, minuteX, minuteY + offset, 0x0000);
  }

  // Piirretään sekuntiviisari
  float secondAngle = (second() * 6) * PI / 180 - PI / 2;
  float secondX = cos(secondAngle) * (SCREEN_WIDTH/2 - 10) + SCREEN_WIDTH/2;
  float secondY = sin(secondAngle) * (SCREEN_HEIGHT/2 - 10) + SCREEN_WIDTH/2;
  frame.drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, secondX, secondY, TFT_RED);

  // Piirretään kellon keskusta
  frame.fillSmoothCircle((SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), 5, 0x0210, 0x0210);
}

void drawApplicationMenu(int x_offset, int y_offset, int app_size) {
  frame.fillSmoothCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, 0x0, 0x0);
  int appId = 0;

  int selectedApp = 0, selectedX = 0, selectedY = 0;
  for(int x_pos = 0; x_pos < 3; x_pos++)
  {
    for(int y_pos = 0; ((y_pos < 3 && (x_pos % 2 == 0)) || (y_pos < 4 && !(x_pos % 2 == 0))) ; y_pos++)
    {
        int x = menu_x + (x_pos * (app_size + x_offset));
        int y = menu_y + (y_pos * (app_size + y_offset)); 
        int bonus_offset = 0;
        if(x_pos % 2 == 0){bonus_offset = app_size/2;}
        if( !(selectedApplication != -1 && selectedApplication == appId))
        {
           frame.fillSmoothCircle(x, y + bonus_offset, (app_size/2) , menuColors[appId], menuColors[appId]);
           if(appId == 2)
           {

            for(int i = 0; i < 12; i++) {
            float angle = i * 30;
            float x1 = cos(angle * PI / 180) * (app_size/2 ) + x;
            float y1 = sin(angle * PI / 180) * (app_size/2 ) +  y + bonus_offset;
            float x2 = cos(angle * PI / 180) * (app_size/2 - 4) + x;
            float y2 = sin(angle * PI / 180) * (app_size/2 - 4) +  y + bonus_offset;

            // Piirretään paksumpia viivoja
            for (int offset = -1; offset <= 1; offset++) {
              frame.drawLine(x1 + offset, y1, x2 + offset, y2, 0x00);
              frame.drawLine(x1, y1 + offset, x2, y2 + offset, 0x00);
            }
            }

      // Piirretään tuntiviisari
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; // Tuntiviisari liikkuu myös minuuttien mukaan
  float hourX = cos(hourAngle) * (app_size/2 - 17) +x; // Lyhyempi
  float hourY = sin(hourAngle) * (app_size/2 - 17)  + y + bonus_offset; // Lyhyempi
  for (int offset = -2; offset <= 2; offset++) { // Paksumpi
    frame.drawLine(x + offset, y + bonus_offset, hourX + offset, hourY, 0x0000);
    frame.drawLine(x, y + bonus_offset + offset, hourX, hourY + offset, 0x0000);
  }


            // Piirretään minuuttiviisari
  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * (app_size/2 - 13) + x; // Lyhyempi kuin sekuntiviisari
  float minuteY = sin(minuteAngle) * (app_size/2 - 13) + y + bonus_offset; // Lyhyempi kuin sekuntiviisari
  for (int offset = -1; offset <= 1; offset++) { // Paksumpi kuin sekuntiviisari
    frame.drawLine(x + offset, y + bonus_offset, minuteX + offset, minuteY, 0x0000);
    frame.drawLine(x, y + bonus_offset + offset, minuteX, minuteY + offset, 0x0000);
  }

             // Piirretään sekuntiviisari
            float secondAngle = (second() * 6) * PI / 180 - PI / 2;
            float secondX = cos(secondAngle) * (app_size/2 - 10) + x;
            float secondY = sin(secondAngle) * (app_size/2 - 10) + y + bonus_offset;
            frame.drawLine(x, y + bonus_offset, secondX, secondY, TFT_RED);

            frame.fillSmoothCircle(x, y + bonus_offset, 1, 0x0210, 0x0210);
           }
        }
        else
        {
         selectedApp = appId, selectedX = x, selectedY = y + bonus_offset;
        
        }
        appId++;
    }
  }
  //selected background
  frame.fillSmoothCircle(selectedX , selectedY , (app_size/2 + 5)*(1+((float)scaleAnim/5)) , 0xFFFFFF, 0xFFFFFF);
  frame.fillSmoothCircle(selectedX, selectedY, (app_size/2)*(1+((float)scaleAnim/5)) , menuColors[selectedApp], menuColors[selectedApp]);
  if(selectedApplication == 2)
  {
    for(int i = 0; i < 12; i++) {
    float angle = i * 30;
    float x1 = cos(angle * PI / 180) * (app_size/2)*(1+((float)scaleAnim/5)) + selectedX;
    float y1 = sin(angle * PI / 180) * (app_size/2)*(1+((float)scaleAnim/5)) + selectedY;
    float x2 = cos(angle * PI / 180) * ((app_size/2)*(1+((float)scaleAnim/5))-4) + selectedX;
    float y2 = sin(angle * PI / 180) * ((app_size/2)*(1+((float)scaleAnim/5))-4) +  selectedY;

    // Piirretään paksumpia viivoja
    for (int offset = -1; offset <= 1; offset++) {
    frame.drawLine(x1 + offset, y1, x2 + offset, y2, 0x00);
    frame.drawLine(x1, y1 + offset, x2, y2 + offset, 0x00);
    }
    }

    // Piirretään tuntiviisari
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; // Tuntiviisari liikkuu myös minuuttien mukaan
  float hourX = cos(hourAngle) * ((app_size/2 - 11)*(1+((float)scaleAnim/5))) +selectedX; // Lyhyempi
  float hourY = sin(hourAngle) * ((app_size/2 - 11)*(1+((float)scaleAnim/5)))  + selectedY; // Lyhyempi
  for (int offset = -2; offset <= 2; offset++) { // Paksumpi
    frame.drawLine(selectedX + offset,selectedY, hourX + offset, hourY, 0x0000);
    frame.drawLine(selectedX, selectedY + offset, hourX, hourY + offset, 0x0000);
  }

            // Piirretään minuuttiviisari
  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * ((app_size/2 - 7)*(1+((float)scaleAnim/5)) ) + selectedX; // Lyhyempi kuin sekuntiviisari
  float minuteY = sin(minuteAngle) * ((app_size/2 - 7)*(1+((float)scaleAnim/5)) ) + selectedY; // Lyhyempi kuin sekuntiviisari
  for (int offset = -1; offset <= 1; offset++) { // Paksumpi kuin sekuntiviisari
    frame.drawLine(selectedX + offset, selectedY, minuteX + offset, minuteY, 0x0000);
    frame.drawLine(selectedX, selectedY + offset, minuteX, minuteY + offset, 0x0000);
  }

    // Piirretään sekuntiviisari
            float secondAngle = (second() * 6) * PI / 180 - PI / 2;
            float secondX = cos(secondAngle) * ((app_size/2-3)*(1+((float)scaleAnim/5))) + selectedX;
            float secondY = sin(secondAngle) * ((app_size/2-3)*(1+((float)scaleAnim/5))) + selectedY;
            frame.drawLine(selectedX, selectedY, secondX, secondY, TFT_RED);

    frame.fillSmoothCircle(selectedX, selectedY, 1.2*(1+((float)scaleAnim/5)), 0x0210, 0x0210);
  }

  if(isDragging)
  {
  frame.fillSmoothCircle(120, 120, app_size/6 , 0xFFFFFF, 0xFFFFFF);
  }
}

void handleApplicationMenuTouch(int x_offset, int y_offset, int app_size) {
  if (touch.userTouch() && scaleAnim == 1) {
    last_touch = millis();
    if (!isDragging) {
      touchStartX = touch.x;
      touchStartY = touch.y;
      //selectedApplication = -1;
      userWake = true;
      isDragging = true;
    } else {
      // Relative movement
      menu_x += (touch.x - touchStartX);
      menu_y += (touch.y - touchStartY);
      touchStartX = touch.x;
      touchStartY = touch.y;
      Serial.println("mouseX: "+String(menu_x)+", mouseY:"+String(menu_y));
    }
  } else {
    //User has released the touch
    
    if(isDragging)
    {

      if(last_release + 1000 > millis() && menu_x == selectPosX && menu_y == selectPosY && selectedApplication == 2)
      {
        Serial.println("start app");
        openApp = true;
      }
      last_release = millis();
      selectPosX = menu_x, selectPosY = menu_y;
      getNearestApplication(x_offset, y_offset, app_size);
    }
    isDragging = false;
  }

  if (!isDragging && last_touch + 100 < millis()) { //SmoothAjo6000
    SmoothAjo6000(near_app_x,near_app_y);//) {
     // selectedApplication = next_app;
      //Ready
    //}
}
}

void getNearestApplication(int x_offset, int y_offset, int app_size) {
  int current_xpos = (0-menu_x) + 100, current_ypos = (0-menu_y) + 100;
  int nearest_app_x, nearest_app_y, nearest_dist = 1000, nearest_app = -1;
  int appId = 0;
  for(int x_pos = 0; x_pos < 3; x_pos++)
  {
    for(int y_pos = 0; ((y_pos < 3 && (x_pos % 2 == 0)) || (y_pos < 4 && !(x_pos % 2 == 0))) ; y_pos++)
    {

       int bonus_offset = 0;
        if(x_pos % 2 == 0){bonus_offset = app_size/2;}

      //int app_x = x_pos + (app_size/2), app_y = y_pos + (app_size/2) + bonus_offset;

      
      int app_x = x_pos * (app_size + x_offset);// + (app_size/2);
      int app_y = y_pos * (app_size + y_offset)+bonus_offset; // (app_size/2) 
      int dist = sqrt(pow(current_xpos - app_x, 2) + pow(current_ypos - app_y, 2));
      if(dist < nearest_dist)
      {
        nearest_app = appId;
        nearest_dist = dist;
        nearest_app_x = app_x;
        nearest_app_y = app_y;
      }
      appId++;
    }
  }
  near_app_x = (0-nearest_app_x) + 120;
  near_app_y = (0-nearest_app_y) + 120;
  selectedApplication = nearest_app;
  Serial.println("Nearest application x:"+String(near_app_x)+", y:"+String(near_app_y));
}

void SmoothAjo6000(int target_xpos, int target_ypos) {
  int speed = 5, xdistance = abs(target_xpos - menu_x), ydistance = abs(target_ypos - menu_y),xspeed, yspeed;
  float ratio;
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
        if (menu_x != target_xpos) {
            if (menu_x > target_xpos) {
                menu_x -= xspeed;
                if (menu_x < target_xpos) menu_x = target_xpos;
            } else if (menu_x < target_xpos) {
                menu_x += xspeed;
                if (menu_x > target_xpos) menu_x = target_xpos;
            }
        }
        if (menu_y != target_ypos) {
            if (menu_y > target_ypos) {
                menu_y -= yspeed;
                if (menu_y < target_ypos) menu_y = target_ypos;
            } else if (menu_y < target_ypos) {
                menu_y += yspeed;
                if (menu_y > target_ypos) menu_y = target_ypos;
            }
        }
    }
   // return (menu_x == target_xpos && menu_y == target_ypos);
}