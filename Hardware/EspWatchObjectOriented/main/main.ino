#include <TFT_eSPI.h>
#include <TimeLib.h> // (Time by Michael Margolis).
#include "touch.h"
#include "heartrate.h"
#include "gyroscope.h"
#include "pcf.h"
#include "debug.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define SCREEN_CALCULATED 57600 // 240 * 240

#define GyroIntPin 23
#define LCD_BACKLIGHT 32
#define VOLTAGE 35

#define SCREEN_SLEEP_BRIGHTNESS 10 //%
int SCREEN_BRIGHTNESS=100; //%

TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite frame = TFT_eSprite(&lcd);
TOUCH touch;
HEARTRATE heartrate;
GYROSCOPE gyroscope;
PCF pcf;
DEBUG debug(heartrate, gyroscope, pcf);
int pulseAnim = 14;


enum App
{
  HOME = 0,
  MENU = 1,
  SETTINGS = 2,
  PULSE = 3
};

bool openApp = true;
int scaleAnim = 15;
int selectPosX = -1, selectPosY = -1;
int menu_x = 55, menu_y = 50, next_app = -1;
App application = HOME;
int applicationPage = 1;
int applicationMinPage = 0;
int applicationMaxPage = 1;
int selectedApplication = 4;
int near_app_x = 55, near_app_y = 50; 
bool userWake = false;
int touchStartX = 0;
int touchStartY = 0;
bool isDragging = false;
unsigned long last_touch = 0;
unsigned long last_release = 0;
bool canPulse = false;
unsigned long last_slide = 0;
float voltage = 0;
constexpr float voltageMultiplier = (3.3 / 4095.0) * 3.32;     // 3.32 is compensation for the voltage divider
int homescreen_theme = 1;

int notifyview_y = 0;
bool notifyopen = false;
bool notifyarefullopen = false;
bool notify_userDrag = false;
int notify_offset = 0;
int notify_startpos = 0;
int notify_target = -1;
unsigned long notify_drag_start = 0;

bool pulseenabled = true;
bool gyroscopeenabled = true;
bool hallenabled = true;

unsigned long lastuserbtn = 0;

unsigned long sleeptime = 0;
int waitsleep = 50000;
int averagePulsePerHour[24] = {-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1};
int lastpulse = 0;
bool sleepmode = false;

void updatePulse(int newpulse) {
  if(lastpulse != newpulse) {
    for(int i=0; i<24; i++) {
      if(averagePulsePerHour[i] == -1) {
        averagePulsePerHour[i] = newpulse;
        break;
      }
    }
    lastpulse = newpulse;
  }
}

String getMaxPulse() {
  int maxPulse = 0;
  for(int i=0; i<24; i++) {
    if(averagePulsePerHour[i] == -1) {
      break;
    }
    else {
      if(maxPulse < averagePulsePerHour[i]) {
        maxPulse = averagePulsePerHour[i];
      }
    }
  }
  if(maxPulse == 0) {
    return "-";
  } else {
    return String(maxPulse);
  }
}

String getMinPulse() {
  int minPulse = 999;
  for(int i=0; i<24; i++) {
    if(averagePulsePerHour[i] == -1) {
      break;
    }
    else {
      if(minPulse > averagePulsePerHour[i]) {
        minPulse = averagePulsePerHour[i];
      }
    }
  }
  if(minPulse == 999) {
    return "-";
  } else {
    return String(minPulse);
  }
}

void setup() {
  Serial.begin(115200);
  touch.init();
  lcd.begin();

  uint8_t trytimes = 100;
  while(true){
    if(heartrate.begin()){canPulse=true; break;}else{if(trytimes>0){trytimes--;}else{break;}}
  }
  trytimes = 100;
  while (true) {
    if (gyroscope.begin()){break;}else{if(trytimes>0){trytimes--;}else{break;}}
  }
  trytimes = 100;
  while (true) {
    if (pcf.begin()){break;}else{if(trytimes>0){trytimes--;}else{break;}}
  }


  lcd.setRotation(1);
  lcd.fillScreen(0);
  frame.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT, TFT_TRANSPARENT);
  last_release = millis(); 
  sleeptime = millis();
  setTime(0, 0, 00, 1, 12, 2024);
  heartrate.enableSensor();
  pinMode(GyroIntPin,INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(GyroIntPin), gyroInterrupt, RISING);
  pinMode(LCD_BACKLIGHT,OUTPUT);
  pinMode(VOLTAGE,INPUT);
  analogWrite(LCD_BACKLIGHT,(int)(((float)255/100)*SCREEN_BRIGHTNESS));
}

void gyroInterrupt() {
  Serial.println("Gyro interrupt triggered!");
  sleeptime = millis();
}


void loop() {
  //debug.loop();

  voltage = analogRead(VOLTAGE) * voltageMultiplier + 0.4;    // 0.4 is compensation for schottky diode voltage drop
  updatePulse(heartrate.getAvgPulse());
  heartrate.loop();

  if(!(sleeptime + waitsleep < millis()) && sleepmode) {
    sleepmode = false;
    analogWrite(LCD_BACKLIGHT,(int)(((float)255/100)*SCREEN_BRIGHTNESS));
  }

  if(sleeptime + waitsleep < millis() && !sleepmode)
  {
    if(application != MENU && application != HOME) {
      application = MENU;
      openApp = false;
    }

    if(application == HOME) {
      sleepmode = true;
      analogWrite(LCD_BACKLIGHT,(int)(((float)255/100)*SCREEN_SLEEP_BRIGHTNESS));
    }

    if(application == MENU && scaleAnim == 1) {
    
      if(menu_x != 55 || menu_y != 50) {
        near_app_x = 55;
        near_app_y = 50;
        selectedApplication = 4;
      }

      if(menu_x == 55 && menu_y == 50) {
        sleepmode = true;
        openApp = true;
        analogWrite(LCD_BACKLIGHT,(int)(((float)255/100)*SCREEN_SLEEP_BRIGHTNESS));
        notifyview_y = 0;
        notifyopen = false;
      }
    }
  }

  if(heartrate.sensorEnabled && (application == PULSE || application == MENU) && !sleepmode)
  {
    if(heartrate.pulseDetected()){pulseAnim=13;} // Animation can start if 15 turns 14
    //this make heart stay big if pulse not detected
    if(pulseAnim<14) {
      if(pulseAnim > 7) {
        pulseAnim--;
      }
      else {
        pulseAnim = 14; // Reset pulse animation
      }
    }
  }

  if(openApp) {
    if(scaleAnim < 15) {
      scaleAnim++;
    }else{
      if(application == MENU)
      {
        if(selectedApplication == 4)
        {
          applicationMinPage = 0;
          applicationMaxPage = 1;
          applicationPage = homescreen_theme;
          application = HOME;
        }
        else if(selectedApplication == 5)
        {
          sleeptime = millis(); //Cant sleep in pulse app
          applicationMinPage = 0;
          applicationMaxPage = 2;
          applicationPage = 1;
          application = PULSE;
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

  if(touch.userTouch()) {
    sleeptime = millis();
  }

  if(application != MENU && !notifyopen && (last_slide+500) < millis()) {
    if(touch.userSwipeRight()) {
      Serial.println("sovellus oikealle");
      if(applicationPage > applicationMinPage) {
        Serial.println("sovellus oikealle ok");
        applicationPage--;
        if(application == HOME) {
          homescreen_theme = applicationPage;
        }
      }
      else{Serial.println("sovellus oikealle fail");}
      last_slide = millis();
    }
    if(touch.userSwipeLeft()) {
      Serial.println("sovellus vasemmalle");
      if(applicationPage < applicationMaxPage) {
        applicationPage++;
        Serial.println("sovellus vasemmalle ok");
        if(application == HOME) {
          homescreen_theme = applicationPage;
        }
      }
      else{Serial.println("sovellus vasemmalle fail");}
      last_slide = millis();
    }
  }

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
  else if(application == PULSE)
  {
    drawApplicationPulse();
    handleApplicationPulse();
  }
  drawNotifyView();
  handleNotifyView();
  frame.pushSprite(0,0);
}

void drawNotifyBtn(int x, int y, bool btnState, String txt) {
    // btn background
    frame.setTextSize(1);
    frame.setTextColor(TFT_BLACK);
    if(btnState) {
      frame.fillSmoothCircle(x, y, 28, TFT_WHITE, TFT_WHITE);
      frame.fillSmoothCircle(x, y, 26, TFT_GREEN, TFT_GREEN);
      frame.drawCentreString(txt, x, y - 8, 2);
      frame.drawCentreString("Enabled", x, y + 8, 2);
    } else {
      frame.fillSmoothCircle(x, y, 26, TFT_WHITE, TFT_WHITE);
      frame.fillSmoothCircle(x, y, 24, TFT_RED, TFT_RED);
      frame.drawCentreString(txt, x, y - 8, 2);
      frame.drawCentreString("Disabled", x, y + 8, 2);
    }
}

void drawNotifyView() {

if(notifyview_y != 0) {
    frame.fillSmoothCircle(SCREEN_WIDTH/2, notifyview_y - SCREEN_HEIGHT/2, SCREEN_WIDTH/2+4, TFT_WHITE, TFT_WHITE);
    frame.fillSmoothCircle(SCREEN_WIDTH/2, notifyview_y - SCREEN_HEIGHT/2, SCREEN_WIDTH/2, TFT_BLACK, TFT_BLACK);

    if(notifyarefullopen){
    frame.fillRect(0, 0, SCREEN_WIDTH, notifyview_y - SCREEN_HEIGHT/2, TFT_BLACK);
    frame.setTextColor(TFT_WHITE);
    frame.setTextSize(2);
    frame.drawCentreString("No notifications", SCREEN_WIDTH/2, notifyview_y - (SCREEN_HEIGHT/2 + 30), 2);
    }
    if(notifyview_y < SCREEN_HEIGHT)
    {

      drawNotifyBtn(SCREEN_WIDTH/2-63, notifyview_y-12, gyroscopeenabled, "Gyroscope");
      drawNotifyBtn(SCREEN_WIDTH/2, notifyview_y, pulseenabled, "Heartrate");
      drawNotifyBtn(SCREEN_WIDTH/2+63, notifyview_y-12, hallenabled, "Hall");
    }
  }
}



void handleNotifyView() {
  if (application != MENU) {

    if (touch.userTouch()) {
      if(!notify_userDrag) {
        if(notifyview_y != 0 || (notifyview_y == 0 && touch.y < 40) ) { 
          notify_userDrag = true;
          notify_offset = touch.y - notifyview_y;
          notify_startpos = touch.y;
          notify_drag_start = millis();
        }
      } else {
        notifyview_y = touch.y - notify_offset;
      }
      if(notifyview_y>SCREEN_HEIGHT){notifyview_y=SCREEN_HEIGHT;}
      
    }

    if (touch.userReleaseTouch()) {
      if(notify_userDrag){
        int presstime = millis() - notify_drag_start;
        int btn_radius = 15;

        bool btn1_vertical = touch.x > ((SCREEN_WIDTH/2) - btn_radius) && touch.x < ((SCREEN_WIDTH/2) + btn_radius);
        bool btn1_horizontal = touch.y > ((90+13) - btn_radius) && touch.y < ((90+13) + btn_radius);
        bool btn1 = btn1_vertical && btn1_horizontal;

        bool btn2_vertical = touch.x > (((SCREEN_WIDTH/2)-63) - btn_radius) && touch.x < (((SCREEN_WIDTH/2)-63) + btn_radius);
        bool btn2_horizontal = touch.y > ((90+1) - btn_radius) && touch.y < ((90+1) + btn_radius);
        bool btn2 = btn2_vertical && btn2_horizontal;

        bool btn3_vertical = touch.x > (((SCREEN_WIDTH/2)+63) - btn_radius) && touch.x < (((SCREEN_WIDTH/2)+63) + btn_radius);
        bool btn3_horizontal = touch.y > ((90+1) - btn_radius) && touch.y < ((90+1) + btn_radius);
        bool btn3 = btn3_vertical && btn3_horizontal;

        bool can_use = abs(notifyview_y - 90) <3;

        if(presstime < 300 && can_use)
        {
          if(btn1) {
            pulseenabled = !pulseenabled;
          }
          if(btn2) {
            gyroscopeenabled = !gyroscopeenabled;
          }
          if(btn3) {
            hallenabled = !hallenabled;
          }
        }
        notify_userDrag = false;
      }
        if((notify_startpos - touch.y) < 0 && notify_target == -1) {
          int diff = abs(notify_startpos - touch.y);

          if(diff > 50) {
            notify_target = 240;
            notifyarefullopen = true;
          } else {
            notify_target = 90;
            notifyarefullopen = false;
          }
          notifyopen = true;
        }

        if((notify_startpos - touch.y) > 0) {
           notify_target = 0;
            notifyopen = false;
        }
    }

      for(int i=0; i<20; i++) {
        if(notify_target != -1) {
          if(notify_target < notifyview_y) {
            notifyview_y--;
          }
          if(notify_target > notifyview_y) {
            notifyview_y++;
          }
          if(notify_target == notifyview_y) {
            notify_target = -1;
          }
        }
      }
  }
}

uint8_t rgbTo8Bit(uint8_t red, uint8_t green, uint8_t blue) {
  red = red & 0x07;   
  green = green & 0x07; 
  blue = blue & 0x03;  
  return (red << 5) | (green << 2) | blue;
}

uint16_t rgbTo16Bit(uint8_t red, uint8_t green, uint8_t blue) {
  red = (red >> 3) & 0x1F;       // 5 bit red
  green = (green >> 2) & 0x3F;   // 6 bit green
  blue = (blue >> 3) & 0x1F;     // 5 bit blue
  return (red << 11) | (green << 5) | blue;
}
void handleApplicationPulse() {
  if(touch.userSwipeUp() && !notifyopen) { //touch.userSwipeRight()) {
    application = MENU;
    openApp = false;
    Serial.println("close app");
  }
}

void handleApplicationHomeTouch() {
  if(touch.userSwipeUp() && !notifyopen) { // touch.userSwipeRight() 
    application = MENU;
    openApp = false;
    Serial.println("close app");
  }
}

void drawApplicationPulse0page() {
  frame.fillSmoothCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, rgbTo16Bit(50,50,50), rgbTo16Bit(50,50,50));
  frame.setTextColor(TFT_WHITE);

  frame.setTextSize(3);

  frame.drawCentreString("Daily", SCREEN_WIDTH/2, 50 , 1);

  if(averagePulsePerHour[0] != -1) {
  int x_offset = (SCREEN_WIDTH/2) - 80;
  int y_offset = (SCREEN_HEIGHT/2) + 50;
  int margin = 160/24;
  int lastsize = 20;
  int lastx =0;

  String minPulse = getMinPulse();
  String maxPulse = getMaxPulse();

  if(maxPulse != "-") {
    frame.drawWideLine((SCREEN_WIDTH/2) - 80, y_offset - (maxPulse.toInt()/2), (SCREEN_WIDTH/2) + 80, y_offset - (maxPulse.toInt()/2), 1, TFT_BLUE);
  }

  if(minPulse != "-") {
    frame.drawWideLine((SCREEN_WIDTH/2) - 80, y_offset - (minPulse.toInt()/2), (SCREEN_WIDTH/2) + 80, y_offset - (minPulse.toInt()/2), 1, TFT_GREEN);
  }
  

  for(int i=0; i<24; i++) {
    int newsize = averagePulsePerHour[i]; //random(0,80);
    if(newsize == -1){break;}else{newsize = newsize/2;}
    if(i == 0){lastsize = newsize;} // start 
    frame.drawWideLine(x_offset + margin*i, y_offset - lastsize, x_offset + margin + margin*i, y_offset - newsize, 2, TFT_RED);
    lastx = x_offset + margin + margin*i;
    lastsize = newsize;
  }

  frame.drawCircle(lastx + 3,  (y_offset - 1.5) - lastsize, 3, TFT_WHITE);

  frame.setTextSize(1);

  frame.setTextColor(TFT_GREEN);
  
  frame.drawCentreString("Min "+minPulse+"bpm", SCREEN_WIDTH/2, (SCREEN_HEIGHT/2) + 60 , 2);

  frame.setTextColor(TFT_BLUE);
  
  frame.drawCentreString("Max "+maxPulse+"bpm", SCREEN_WIDTH/2, (SCREEN_HEIGHT/2) + 75 , 2);


    //left line
  frame.drawWideLine((SCREEN_WIDTH/2) - 80, (SCREEN_HEIGHT/2) - 20, (SCREEN_WIDTH/2) - 80, (SCREEN_HEIGHT/2) + 50, 2, TFT_WHITE);

  //bottom line
  frame.drawWideLine((SCREEN_WIDTH/2) - 80, (SCREEN_HEIGHT/2) + 50, (SCREEN_WIDTH/2) + 80, (SCREEN_HEIGHT/2) + 50, 2, TFT_WHITE);
  }
  else
  {
    frame.setTextColor(TFT_WHITE);
    frame.setTextSize(2);
    frame.drawCentreString("Data not available", SCREEN_WIDTH/2, (SCREEN_HEIGHT/2) + 10 , 2);
  }
}

void drawApplicationPulse2page() {
  frame.fillSmoothCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, rgbTo16Bit(50,50,50), rgbTo16Bit(50,50,50));
  frame.setTextColor(TFT_WHITE);

  frame.setTextSize(3);

  frame.drawCentreString("Weekly", SCREEN_WIDTH/2, 50 , 1);

  for(int i=0; i<7; i++) {
    int x_offset = (SCREEN_WIDTH/2) - 80;
    int y_offset = (SCREEN_HEIGHT/2) + 50;
    //tässä piirretään 7 neliötä jotka näyttävä viikonpäivien sykemäärät
    int barsize = 10 + i*10;
    frame.fillRect(3+x_offset + (i * 23), y_offset-barsize, 15, barsize, TFT_RED);
  }

  //bottom line
  frame.drawWideLine((SCREEN_WIDTH/2) - 80, (SCREEN_HEIGHT/2) + 50, (SCREEN_WIDTH/2) + 80, (SCREEN_HEIGHT/2) + 50, 2, TFT_WHITE);

  frame.setTextSize(1);

  frame.drawCentreString("Min 20bpm", SCREEN_WIDTH/2, (SCREEN_HEIGHT/2) + 60 , 2);
  frame.drawCentreString("Max 80bpm", SCREEN_WIDTH/2, (SCREEN_HEIGHT/2) + 75 , 2);
}

void drawApplicationBatteryScaleableFirstPage(int x, int y, int width, int height, float scale) {
  frame.fillSmoothCircle(x, y, (width/2) * scale, rgbTo16Bit(50,50,50), rgbTo16Bit(50,50,50));
  
  frame.fillRect(x - 50*scale, y - 25*scale, 100*scale, 50*scale, TFT_BLACK);
  frame.fillRect(x - 65*scale, y - 10*scale, 30*scale, 20*scale, TFT_BLACK);

  float batteryScale = (float)voltage/6;
  
  int batteryWidth = (96 * scale) * batteryScale;
  int batteryX = x - (48 * scale) + (96 * scale) - batteryWidth;
  frame.fillRect(batteryX, y - 22 * scale, batteryWidth, 44 * scale, TFT_GREEN);

  float textScale = 1.5 * scale;
  frame.setTextSize(textScale);

  frame.setTextColor(TFT_WHITE);
  frame.drawCentreString(String(voltage)+"v", x, y + 30 * scale, 2);
}

void drawApplicationPulseScaleableFirstPage(int x, int y, int width, int height, float scale) {
  frame.fillSmoothCircle(x, y, (width/2) * scale, rgbTo16Bit(50,50,50), rgbTo16Bit(50,50,50));
  //tässä pitäisi piirtää sydänkuvake 

  float heart_scale = ((float)scale*0.65) + ((float)pulseAnim/45);
  frame.fillTriangle(x - 26 * heart_scale, y - 26 * heart_scale,      x + 26 * heart_scale, y - 26 * heart_scale,     x, y + 15 * heart_scale, TFT_BLACK);
  frame.fillSmoothCircle(x - 11 * heart_scale, y - 31 * heart_scale, 16 * heart_scale, TFT_BLACK, TFT_BLACK);
  frame.fillSmoothCircle(x + 11 * heart_scale, y - 31 * heart_scale, 16 * heart_scale, TFT_BLACK, TFT_BLACK);

  frame.fillTriangle(x - 21 * heart_scale, y - 21 * heart_scale,      x + 21 * heart_scale, y - 21 * heart_scale,     x, y + 10 * heart_scale, TFT_RED);
  frame.fillSmoothCircle(x - 10 * heart_scale, y - 30 * heart_scale, 14 * heart_scale, TFT_RED, TFT_RED);
  frame.fillSmoothCircle(x + 10 * heart_scale, y - 30 * heart_scale, 14 * heart_scale, TFT_RED, TFT_RED);

  frame.setTextColor(TFT_WHITE);
  float textScale = 2 * scale;
  frame.setTextSize(textScale);

  if(canPulse)
  {
    if(heartrate.wristDetected()){
    frame.drawCentreString(String(heartrate.getAvgPulse())+" bpm", x, y + 30 * scale, 2);
    } else {
      frame.drawCentreString("No wrist", x, y + 25 * scale, 1);
      frame.drawCentreString("detected", x, y + 50 * scale, 1);
    }
  }
  else
  {
    frame.drawCentreString("Failed", x, y + 30 * scale, 2);
  }
}

String addZeros(int value) {
  if(value>9) {
    return String(value);
  }
  else {
    return "0"+String(value);
  }
}

void drawApplicationHomeScaleable0Page(int x, int y, int width, int height, float scale) {

   if(sleepmode)
  {
    frame.fillSmoothCircle(x, y, (width/2) * scale, 0x0, 0x0);
  
    float textScale = 3 * scale;

    frame.setTextColor(TFT_WHITE);
    frame.setTextSize(textScale);
    frame.drawCentreString(String(addZeros(hour()))+":"+String(addZeros(minute())), x, y - 25 * scale, 4);
  }

  if(!sleepmode)
  {
    frame.fillSmoothCircle(x, y, (width/2) * scale, 0x03BF, 0x03BF);//0x03BF, 0x03BF);
  
    float textScale = 3 * scale;

    frame.setTextColor(TFT_BLACK);
    frame.setTextSize(textScale);
    frame.drawCentreString(String(addZeros(hour()))+":"+String(addZeros(minute())), x, y - 25 * scale, 4);
    frame.drawCentreString(String(addZeros(second())), x, y - (25 * scale) + (60 * scale) , 2);
  }

 

}

void drawApplicationHomeScaleableFirstPage(int x, int y, int width, int height, float scale) {
  if(sleepmode)
  {
    frame.fillSmoothCircle(x, y, (width/2) * scale, TFT_BLACK, TFT_BLACK);

  for(int i = 0; i < 12; i++) {
    float angle = i * 30;
    float x1 = cos(angle * PI / 180) * ((width/2 )*scale) + x;
    float y1 = sin(angle * PI / 180) * ((height/2 )*scale) + y;
    float x2 = cos(angle * PI / 180) * ((width/2 - 8)*scale) + x;
    float y2 = sin(angle * PI / 180) * ((height/2 - 8)*scale) + y;

    for (int offset = -1; offset <= 1; offset++) {
      frame.drawLine(x1 + offset, y1, x2 + offset, y2, TFT_WHITE);
      frame.drawLine(x1, y1 + offset, x2, y2 + offset, TFT_WHITE);
    }
  }

     // Piirretään tuntiviisari
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; // Tuntiviisari liikkuu myös minuuttien mukaan
  float hourX = cos(hourAngle) * (width/2 - 50)* scale + x; // Lyhyempi
  float hourY = sin(hourAngle) * (height/2 - 50) * scale + y; // Lyhyempi

  for (int offset = -2; offset <= 2; offset++) { // Paksumpi
    frame.drawLine(x + offset, y, hourX + offset, hourY, TFT_WHITE);
    frame.drawLine(x, y + offset, hourX, hourY + offset, TFT_WHITE);
  }

  // Piirretään minuuttiviisari
  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * (width/2 - 30) * scale + x; // Lyhyempi kuin sekuntiviisari
  float minuteY = sin(minuteAngle) * (height/2 - 30) * scale + y; // Lyhyempi kuin sekuntiviisari

  for (int offset = -1; offset <= 1; offset++) { // Paksumpi kuin sekuntiviisari
    frame.drawLine(x + offset, y, minuteX + offset, minuteY, TFT_WHITE);
    frame.drawLine(x, y + offset, minuteX, minuteY + offset, TFT_WHITE);
  }

  // Piirretään kellon keskusta
  //frame.fillSmoothCircle(x, y, 5 * scale, TFT_WHITE, TFT_WHITE);

  }

  if(!sleepmode)
  {
  frame.fillSmoothCircle(x, y, (width/2) * scale, 0x03BF, 0x03BF); //0x03BF, 0x03BF
  
  for(int i = 0; i < 12; i++) {
    float angle = i * 30;
    float x1 = cos(angle * PI / 180) * ((width/2 )*scale) + x;
    float y1 = sin(angle * PI / 180) * ((height/2 )*scale) + y;
    float x2 = cos(angle * PI / 180) * ((width/2 - 8)*scale) + x;
    float y2 = sin(angle * PI / 180) * ((height/2 - 8)*scale) + y;

    for (int offset = -1; offset <= 1; offset++) {
      frame.drawLine(x1 + offset, y1, x2 + offset, y2, 0x00);
      frame.drawLine(x1, y1 + offset, x2, y2 + offset, 0x00);
    }
  }
  
  

  // Piirretään tuntiviisari
  float hourAngle = ((hour() % 12) * 30 + (minute() / 2)) * PI / 180 - PI / 2; // Tuntiviisari liikkuu myös minuuttien mukaan
  float hourX = cos(hourAngle) * (width/2 - 50)* scale + x; // Lyhyempi
  float hourY = sin(hourAngle) * (height/2 - 50) * scale + y; // Lyhyempi

  for (int offset = -2; offset <= 2; offset++) { // Paksumpi
    frame.drawLine(x + offset, y, hourX + offset, hourY, 0x0000);
    frame.drawLine(x, y + offset, hourX, hourY + offset, 0x0000);
  }

  // Piirretään minuuttiviisari
  float minuteAngle = (minute() * 6) * PI / 180 - PI / 2;
  float minuteX = cos(minuteAngle) * (width/2 - 30) * scale + x; // Lyhyempi kuin sekuntiviisari
  float minuteY = sin(minuteAngle) * (height/2 - 30) * scale + y; // Lyhyempi kuin sekuntiviisari

  for (int offset = -1; offset <= 1; offset++) { // Paksumpi kuin sekuntiviisari
    frame.drawLine(x + offset, y, minuteX + offset, minuteY, 0x0000);
    frame.drawLine(x, y + offset, minuteX, minuteY + offset, 0x0000);
  }

  // Piirretään sekuntiviisari
  float secondAngle = (second() * 6) * PI / 180 - PI / 2;
  float secondX = cos(secondAngle) * (width/2 - 40) * scale + x;
  float secondY = sin(secondAngle) * (height/2 - 40) * scale + y;
  frame.drawLine(x, y, secondX, secondY, TFT_RED);

  // Piirretään kellon keskusta
  frame.fillSmoothCircle(x, y, 5 * scale, 0x0210,0x0210);// 0x0210, 0x0210);
  }
}

void drawApplicationHome() {
  if(homescreen_theme == 0)
  {
    drawApplicationHomeScaleable0Page( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  }
  if(homescreen_theme == 1)
  {
    drawApplicationHomeScaleableFirstPage( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  }
}

void drawApplicationPulse() {
  if(applicationPage == 0) {
    drawApplicationPulse0page();
  }
  if(applicationPage == 1) {
    drawApplicationPulseScaleableFirstPage( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  }
  if(applicationPage == 2) {
    drawApplicationPulse2page();
  }
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
           if(appId == 4)
           {
              // tähän piirretään skaalaamaton sovellus ikoni
              if(homescreen_theme == 0) {
                drawApplicationHomeScaleable0Page(x, y + bonus_offset, SCREEN_WIDTH, SCREEN_HEIGHT, 0.3);
              }

              if(homescreen_theme == 1) {
                drawApplicationHomeScaleableFirstPage(x, y + bonus_offset, SCREEN_WIDTH, SCREEN_HEIGHT, 0.3);
              }
           }
           else if(appId == 5)
           {
              // tähän piirretään skaalaamaton sovellus ikoni
              drawApplicationPulseScaleableFirstPage(x, y + bonus_offset, SCREEN_WIDTH, SCREEN_HEIGHT, 0.3);
           }
           else if(appId == 8)
           {
              drawApplicationBatteryScaleableFirstPage(x, y + bonus_offset, SCREEN_WIDTH, SCREEN_HEIGHT, 0.3);
           }
           /*else
           {
              //joku muu kuin kellosovellus
              frame.fillSmoothCircle(x, y + bonus_offset, (app_size/2) ,0x0210,0x0210);
           }*/
        }
        else
        {
         selectedApp = appId, selectedX = x, selectedY = y + bonus_offset;
        
        }
        appId++;
    }
  }
  //selected background
  float scaleval = (float)(scaleAnim)/15; if(scaleval<0.3){scaleval=0.3;}
  frame.fillSmoothCircle(selectedX , selectedY , (SCREEN_WIDTH/2 + 10)*scaleval , 0xFFFFFF, 0xFFFFFF);
  frame.fillSmoothCircle(selectedX, selectedY, (SCREEN_WIDTH/2)*scaleval , 0x0210,0x0210); // 0x03BF 0x0210
  
  if(selectedApplication == 4)
  {
    if(homescreen_theme == 1) {
      drawApplicationHomeScaleableFirstPage(selectedX, selectedY, SCREEN_WIDTH, SCREEN_HEIGHT, scaleval);
    }
    if(homescreen_theme == 0) {
      drawApplicationHomeScaleable0Page(selectedX, selectedY, SCREEN_WIDTH, SCREEN_HEIGHT, scaleval);
    }
  }
  if(selectedApplication == 5)
  {
    drawApplicationPulseScaleableFirstPage(selectedX, selectedY, SCREEN_WIDTH, SCREEN_HEIGHT, scaleval);
  }
  if(selectedApplication == 8)
  {
    drawApplicationBatteryScaleableFirstPage(selectedX, selectedY, SCREEN_WIDTH, SCREEN_HEIGHT, scaleval);
  }
 
  if(isDragging)
  {
  frame.fillSmoothCircle(120, 120, app_size/6 , 0xFFFFFF, 0xFFFFFF);
  }
}

void handleApplicationMenuTouch(int x_offset, int y_offset, int app_size) {

  if (touch.userTouch() && scaleAnim < 1.5) {
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

      if(last_release + 1000 > millis() && abs(menu_x-selectPosX)<5 && abs(menu_y-selectPosY)<5 && (selectedApplication == 4 || selectedApplication == 5))
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
    SmoothAjo6000(near_app_x,near_app_y);
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

      int app_x = x_pos * (app_size + x_offset);// + (app_size/2);
      int app_y = y_pos * (app_size + y_offset)+bonus_offset; // (app_size/2) 
      int dist = sqrt(pow(current_xpos - app_x, 2) + pow(current_ypos - app_y, 2));
      if(dist < nearest_dist)
      {
        if(appId == 4 || appId == 5 || appId == 8) {
          nearest_app = appId;
          nearest_dist = dist;
          nearest_app_x = app_x;
          nearest_app_y = app_y;
        }
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

// TODO: Add libraries to the project files