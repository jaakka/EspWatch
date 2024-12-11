#include "lcd.h"
#include "touch.h"

LCD lcd;
TOUCH touch;
unsigned long lastreset = 0;

void setup() {
    Serial.begin(115200);
    //Init devices
    lcd.init();
    touch.init();

    //Boot screen
    lcd.fillScreen(TFT_WHITE);
    lcd.drawCircle(43, 54, 8, TFT_RED);
    //some bug, draws mirrow text ?
    lcd.drawText("Hello World!", 10, 60, 2, TFT_BLACK);
}

void loop() {
  touch.loop();

  if(touch.userTouch()) {
    Serial.println("x: "+String(touch.x));
    lcd.drawCircle(touch.x, touch.y, 10, TFT_BLACK);
  }

  if(millis() > lastreset){
    lcd.fillScreen(TFT_WHITE);
  lastreset = millis() + 10000;
  }
}