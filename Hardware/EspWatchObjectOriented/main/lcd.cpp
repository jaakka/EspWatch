#include "lcd.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

#define LCD_BACKLIGHT 32

LCD::LCD() : tft(TFT_eSPI()) {
    Serial.println("LCD init");
    tft.begin();
    tft.setRotation(3); // Varmista, että näytön suunta on oikein
    tft.fillScreen(TFT_BLACK);
}

void LCD::drawText(String text, int x, int y, int size, int color) {
    tft.setTextSize(size);
    tft.setTextColor(color);
    tft.setCursor(x, y);
    tft.print(text);
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