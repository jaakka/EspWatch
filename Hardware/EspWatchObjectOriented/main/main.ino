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

const int SCREEN_WIDTH = 240;  // Screen width
const int SCREEN_HEIGHT = 240;  // Screen height

#define GyroIntPin      23 // Gyroscope interrupt pin
//#define TouchIntPin       19 // Lcd interrupt pin
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
const int APP_COUNT = 3;
App* apps[APP_COUNT] = {new MenuApp(), new HomeApp(), new PulseApp()};
int runningApp = 0; // HomeApp is the default start app

// Global variables 
float batteryVoltage = 0;
int userScreenBrightness = SCREEN_WAKE_BRIGHTNESS;
bool watchAwake = true;
bool watchReadyToSleep = false;

void tryStartModules() {
  for (uint8_t i = 0; i < TRY_TIMES_FOR_DEVICES; i++) { 
    if(heartrate.begin()) {
      break;
    } 
  }
  for (uint8_t i = 0; i < TRY_TIMES_FOR_DEVICES; i++) { 
    if(gyroscope.begin()) {
      break;
    } 
  }
  for (uint8_t i = 0; i < TRY_TIMES_FOR_DEVICES; i++) { 
    if(pcf.begin()) {
      break;
    } 
  }
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
  int lcdBrightness = analogRead(LcdBacklightPin);

  if(watchAwake) {
    int newBrightness = (255 * userScreenBrightness) / 100;

    if(watchReadyToSleep) {
      newBrightness = (255 * SCREEN_BEFORE_SLEEP_BRIGHTNESS) / 100;
    }

    if(lcdBrightness < newBrightness) {
      analogWrite(LcdBacklightPin, lcdBrightness + 1);
    } else if(lcdBrightness > newBrightness) {
      analogWrite(LcdBacklightPin, lcdBrightness - 1);
    }
  }

  if(!watchAwake) { 
    if(lcdBrightness > 0) {
      analogWrite(LcdBacklightPin, lcdBrightness - 1);
    } 
  }
} 

void handleApplications() {
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
  //gpio_wakeup_enable(GPIO_NUM_19, GPIO_INTR_LOW_LEVEL);       // Lisätään herätys touch int pinnistä
  //gpio_wakeup_enable(GPIO_NUM_23, GPIO_INTR_HIGH_LEVEL);      // Lisätään herätys gyro int pinnistä
  //esp_sleep_enable_gpio_wakeup();
  //Serial.println("lepotilaan siirtyminen");
  delay(1000);
  //esp_light_sleep_start();
}

void allTimeLoop() { 
  // this is performed continuously, regardless of the state of the clock

  LcdBrightnessSmoothController();
}

void publishFrame() {
  frame.pushSprite(0,0);
}

void onWakeLoop() {
  // this is only performed when the watch is awake
  handleApplications();
  publishFrame();
}

void onSleepLoop() {
  // this is only performed when the watch is sleep mode
}

void wakeUpFromSleep() {
  // this is executed once when the clock wakes up
}

void goToSleep() {
  // this is executed once when the clock goes to sleep
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