#include "lcd.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

#define LCD_BACKLIGHT 32

LCD::LCD() : tft(TFT_eSPI()) {}

void LCD::init(){
    Serial.println("LCD init");
    tft.begin();
    tft.setRotation(0); // Varmista, että näytön suunta on oikein
    tft.fillScreen(TFT_BLACK);
}

void LCD::drawText(String text, int x, int y, int size, int color) {
    tft.setTextColor(color);
   // tft.setTextDatum(TR_DATUM); 
    tft.drawString(text,x,y,size);
}

void LCD::drawBox(int x, int y, int w, int h, int color) {
    tft.fillRect(x, y, w, h, color);
}

void LCD::drawCircle(int x, int y, int r, int color) {
    tft.fillCircle(x, y, r, color);
}

void LCD::drawLine(int x0, int y0, int x1, int y1, int color) {
    tft.drawLine(x0, y0, x1, y1, color);
}

void LCD::fillScreen(int color) {
    tft.fillScreen(color);
}