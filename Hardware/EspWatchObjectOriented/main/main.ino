#include "lcd.h"
#include "touch.h"
LCD lcd;
TOUCH touch;
unsigned long last_update = 0;
int lastAngle = -1;

int last_page_pos = 0;

int lastSecondAngle = -1;
int lastMinuteAngle = -1;
int lastHourAngle = -1;

void setup() {
    Serial.begin(115200);
    lcd.init();
    touch.init();

    lcd.drawCircle(120, 120, 120, RGB(0,200,0) );

    for (int i=0; i<12; i++) {
      lcd.drawLineAngle(120, 120, i * 30, 120,RGB(0,0,0) , 1);
    }
    
    lcd.drawCircle(120, 120, 100, RGB(0,255,0) );
    lcd.drawCircle(120, 120, 8, RGB(200,0,0) );
}

void updateAngle(int newSecondAngle, int newMinuteAngle, int newHourAngle) {
    // Clear previous hands
    lcd.drawLineAngle(120, 120, lastSecondAngle, 90, RGB(0, 255, 0), 1);
    lcd.drawLineAngle(120, 120, lastMinuteAngle, 70, RGB(0, 255, 0), 1);
    lcd.drawLineAngle(120, 120, lastHourAngle, 50, RGB(0, 255, 0), 1);

    // Draw new hands
    lcd.drawLineAngle(120, 120, newSecondAngle, 90, RGB(255, 0, 0), 1);
    lcd.drawLineAngle(120, 120, newMinuteAngle, 70, RGB(0, 0, 0), 1);
    lcd.drawLineAngle(120, 120, newHourAngle, 50, RGB(0, 0, 0), 1);

    // Draw center circle
    lcd.drawCircle(120, 120, 8, RGB(200, 0, 0));
    lcd.drawCircle(120, 120, 6, RGB(255, 0, 0));

    // Update last angles
    lastSecondAngle = newSecondAngle;
    lastMinuteAngle = newMinuteAngle;
    lastHourAngle = newHourAngle;
}

void loop() {
  touch.loop();
    unsigned long currentMillis = millis();
    float seconds = (currentMillis % 60000) / 1000.0; // Laske sekunnit tarkasti
    float minutes = (currentMillis % 3600000) / 60000.0; // Laske minuutit tarkasti
    float hours = (currentMillis % 43200000) / 3600000.0; // Laske tunnit tarkasti

    float secondAngle = (360.0 / 60.0) * seconds; // Laske sekuntiviisarin kulma
    float minuteAngle = (360.0 / 60.0) * minutes; // Laske minuuttiviisarin kulma
    float hourAngle = (360.0 / 12.0) * hours; // Laske tuntiviisarin kulma

    if ((lastSecondAngle != secondAngle || lastMinuteAngle != minuteAngle || lastHourAngle != hourAngle) && last_update < millis()) {
        updateAngle(secondAngle, minuteAngle, hourAngle);
        last_update = millis() + 100;
    }

    /*if(touch.userTouch())
    {
      if(last_page_pos != touch.x)
      {
        lcd.drawCircle(120 + last_page_pos, 120, 100, RGB(0,0,0));
        lcd.drawCircle(120 + touch.x, 120, 100, RGB(255,0,0));
        last_page_pos =  touch.x;
      }
    }*/
}