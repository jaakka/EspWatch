// Libraries
#include <TFT_eSPI.h>
#include <TimeLib.h> // (Time by Michael Margolis).

// Custom classes
#include "touch.h"
#include "heartrate.h"
#include "gyroscope.h"
#include "pcf.h"
#include "debug.h"

// Watch applications 
#include "app.h"
#include "pulseapp.h"
#include "homeapp.h"
#include "menuapp.h"
#include "calculatorapp.h"

const int SCREEN_WIDTH = 240;  // Screen width
const int SCREEN_HEIGHT = 240;  // Screen height

#define GyroIntPin      23 // Gyroscope interrupt pin
//#define TouchIntPin   19 // Lcd interrupt pin
#define LcdBacklightPin 32 // Lcd backlight pin
#define VoltagePin      35 // Battery voltage pin

#define SCREEN_BEFORE_SLEEP_BRIGHTNESS 10  // 0 - 100% 
#define SCREEN_WAKE_BRIGHTNESS 100  // 0 - 100%

#define DELAY_TO_DIM 5000     // Delay to sleep in ms
#define DELAY_TO_SLEEP 10000  // Delay to full sleep in ms

#define TRY_TIMES_FOR_DEVICES 100   // How many times try to start devices

TFT_eSPI lcd = TFT_eSPI();              // For lcd
TFT_eSprite frame = TFT_eSprite(&lcd);  // For virtual lcd, build frame ready before send lcd
TOUCH touch;                            // Lcd touch control
HEARTRATE heartrate;                    // Pulse sensor in daughterboard
GYROSCOPE gyroscope;                    // Gyroscope in daughterboard
PCF pcf;                                // Pcf in daughterboard, connected to light and hall sensors.
DEBUG debug(heartrate, gyroscope, pcf);

// Application handling
const int APP_COUNT = 4;
App* apps[APP_COUNT] = {new MenuApp(), new HomeApp(), new PulseApp(), new CalculatorApp()};
int runningApp = 1; // HomeApp is the default start app

// Global variables 
float batteryVoltage = 0;
int userScreenBrightness = SCREEN_WAKE_BRIGHTNESS;
bool watchAwake = true;
bool watchReadyToSleep = false;
bool watchSleeping = false;
int bat_anim = 0;
int bat_lev = 0;
constexpr float voltageMultiplier = (3.3 / 4095.0) * 3.32;     // 3.32 is compensation for the voltage divider
bool exit_application = false;
unsigned long last_act = 0, last_wakeup = 0;

void tryStartModules() {

  uint8_t i = 0;
  do {
    if(heartrate.begin()) {
      break;
    } 
    i++;
  } while (i < TRY_TIMES_FOR_DEVICES);

  i = 0;
  do {
    if(gyroscope.begin()) {
      break;
    } 
    i++;
  } while (i < TRY_TIMES_FOR_DEVICES);

  i = 0;
  do {
    if(pcf.begin()) {
      break;
    } 
    i++;
  } while (i < TRY_TIMES_FOR_DEVICES);
}

void initPins() {
  //attachInterrupt(digitalPinToInterrupt(GyroIntPin), gyroInterrupt, RISING);
  pinMode(LcdBacklightPin, OUTPUT);
  pinMode(VoltagePin, INPUT);
  analogWrite(LcdBacklightPin, (255 * SCREEN_WAKE_BRIGHTNESS) / 100);
}

void initValues() {
  setTime(0, 15, 00, 1, 1, 2025);
}

void initScreen() {
  lcd.begin();
  touch.init();
  lcd.setRotation(1);
  lcd.fillScreen(0);
  frame.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT, TFT_TRANSPARENT);
}

void initApplications() {
  for (int i = 0; i < APP_COUNT; i++) {
    apps[i]->init();
  }
}

void LcdBrightnessSmoothController() {
  if(!watchSleeping) {
    int sensor = pcf.readLightSensor();
    sensor = map(sensor, 5, 160, 10, 255);
    if(sensor>255){sensor=255;} else if(sensor<10){sensor=10;}
    analogWrite(LcdBacklightPin, sensor);
    userScreenBrightness = sensor;
  }
} 

void handleApplications() {
  if(exit_application) {
    runningApp = 0; // Open menu
  }
  for (int i = 0; i < APP_COUNT; i++) {
    if(i == runningApp) {
      // Handle open application
      apps[i]->handleApplication();
      apps[i]->drawApplication();
    } else {
      // Handle background applications
      apps[i]->handleApplicationBackground();
    }
  }
}

void onBoot() {
  // this is executed once when the clock boots up
  Serial.begin(115200);
  initValues();
  initPins();
  initScreen();
  tryStartModules();
  initApplications();
  pcf.enableLightSensor();
}

void allTimeLoop() { 
  // this is performed continuously, regardless of the state of the clock
  if(!watchSleeping && (last_act+10000 < millis())) {
    goToSleep();
  }
  LcdBrightnessSmoothController();
}

void publishFrame() {
  frame.pushSprite(0,0);
}

void drawProgressArc(int x, int y, int radius, int start_angle, int end_angle, uint16_t color, int thickness) {
  for (int t = 0; t < thickness; t++) {
    for (int angle = start_angle; angle < end_angle; angle++) {
      int x1 = x + (radius - t) * cos(angle * DEG_TO_RAD);
      int y1 = y + (radius - t) * sin(angle * DEG_TO_RAD);
      int x2 = x + (radius - t - 1) * cos(angle * DEG_TO_RAD);
      int y2 = y + (radius - t - 1) * sin(angle * DEG_TO_RAD);
      frame.drawLine(x1, y1, x2, y2, color);
    }
  }
}

void onWakeLoop() {
  // this is only performed when the watch is awake
  if(last_wakeup+1000 < millis()){touch.loop();}
  if(touch.userTouch()){last_act = millis();}
  handleApplications();
  tempChargeAnim();
  publishFrame();
}

void tempChargeAnim(){
  float voltage = analogRead(VoltagePin) * voltageMultiplier + 0.4;    // 0.4 is compensation for schottky diode voltage drop
  
  if (voltage > 5) {
  if (bat_anim < 100) {
    bat_anim+=2;
  }
  else
  {
    bat_anim=bat_lev; 
    bat_lev++;
  }

    if(bat_lev>100){ bat_lev=0;}
    drawProgressArc(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH/2, 0, ((float)360/100) * bat_anim, rgb(155,155,155), 3);
    drawProgressArc(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH/2, 0, ((float)360/100) * bat_lev, rgb(0,255,0), 3);
  }
}

void onSleepLoop() {
  // this is only performed when the watch is sleep mode
}

void wakeUpFromSleep() {
  // this is executed once when the clock wakes up
  heartrate.enableSensor();
  pcf.enableLightSensor();
  unDimLcdBeforeSleep();
  watchSleeping = false;
  last_act = millis();
  last_wakeup = millis();
}

void dimLcdBeforeSleep() {
  int bright = userScreenBrightness;
  while(bright > 0) {
    bright--;
    analogWrite(LcdBacklightPin, bright);
    delay(10);
  }
}

void unDimLcdBeforeSleep() {
  int sensor = pcf.readLightSensor();
  sensor = map(sensor, 5, 160, 10, 255);
  if(sensor>255){sensor=255;} else if(sensor<10){sensor=10;}
  int bright = 0;
  while(bright < sensor) {
    bright++;
    analogWrite(LcdBacklightPin, bright);
    delay(10);
  }
}

void goToSleep() {
  // this is executed once when the clock goes to sleep
    watchSleeping = true; // this disables screen light controller
    dimLcdBeforeSleep();
    heartrate.disableSensor();
    pcf.disableLightSensor();
    gpio_wakeup_enable(GPIO_NUM_19, GPIO_INTR_LOW_LEVEL);       //  Lisätään herätys touch int pinnistä
    //gpio_wakeup_enable(GPIO_NUM_23, GPIO_INTR_HIGH_LEVEL);      // Lisätään herätys gyro int pinnistä
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
    wakeUpFromSleep();
}

void gyroInterrupt() {
  // this function is executed when the user makes a significant movement
  if(!watchAwake) { 
    wakeUpFromSleep();
    watchAwake = true;
  }
}

void setup() {
  onBoot();
}

void loop() {
  allTimeLoop();

  if(watchAwake) {
    onWakeLoop();
  }
   
  if(!watchAwake) {
    onSleepLoop();
  }
}
