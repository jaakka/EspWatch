#include "lcd.h"
#include "touch.h"

LCD lcd;
TOUCH touch;

int last_page_pos = 0;
unsigned long last_update = 0;
unsigned long last_move= 0;
int slide_pos = 0;
bool isDragging = false;
int size = 240;

void setup() {
    Serial.begin(115200);
    lcd.init();
    touch.init();

    lcd.drawCircle(120, 120, 120, RGB(0, 200, 0));
}

void loop() {
    

    if (millis() > last_update) {
        touch.loop();

        lcd.fillScreen(RGB(0, 0, 0)); // Tyhjennä näyttö
        lcd.drawCircle(260 + slide_pos, 120, size/2, RGB(255, 0, 0));
        lcd.drawCircle(520 + slide_pos, 120, size/2, RGB(0, 255, 0));
        lcd.drawCircle(slide_pos, 120, size/2, RGB(0, 0, 255));
        last_update = millis() + 10;
    }

    if (touch.userTouch()) {
        if (!isDragging) {
            isDragging = true;
            last_page_pos = touch.x;
        }

        int delta = touch.x - last_page_pos;
        slide_pos += delta * 2;
        last_page_pos = touch.x;
        last_move = millis();

    } else {
        isDragging = false;
    }

    if( (last_move + 100) < millis() )
    {
      if(slide_pos < -270){ if(slide_pos > -400){slide_pos --;} if(slide_pos < -400){slide_pos ++;} } // Green page
      if(slide_pos < -20 && slide_pos > -269){ if(slide_pos > -140){slide_pos --;} if(slide_pos < -140){slide_pos ++;} } // Red page
      if(slide_pos > -19){ if(slide_pos > 120){slide_pos --;} if(slide_pos < 120){slide_pos ++;} } // Blue page
      if(size < 240){size ++;}
    }
    else
    {
      if(size > 190){size --;}
    }

    Serial.println(slide_pos);
}