#include "lcd.h"

LCD lcd;

void setup() {
    //Boot screen
    lcd.fillScreen(TFT_WHITE);
  
    //some bug, draws mirrow text ?
    lcd.drawText("Hello World!", 10, 60, 2, TFT_BLACK);
}

void loop() {

}