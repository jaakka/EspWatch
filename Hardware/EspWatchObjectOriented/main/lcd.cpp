#include "lcd.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

#define LCD_BACKLIGHT 32

LCD::LCD() : tft(TFT_eSPI()) {}

void LCD::init(){
    Serial.println("LCD init");
    tft.begin();
    tft.setRotation(0); // Varmista, että näytön suunta on oikein
     // näytön nopeutta nostettiin 40000000
    tft.fillScreen(TFT_BLACK);
}

void LCD::drawText(String text, int x, int y, int size, RGB color) {
    tft.setTextColor(color.getValue());
    tft.drawString(text,x,y,size);
}

void LCD::drawBox(int x, int y, int w, int h, RGB color) {
    tft.fillRect(x, y, w, h, color.getValue());
}

void LCD::drawCircle(int x, int y, int r, RGB color) {
    tft.fillCircle(x, y, r, color.getValue());
}

void LCD::drawLine(int x0, int y0, int x1, int y1, RGB color) {
    tft.drawLine(x0, y0, x1, y1, color.getValue());
}

void LCD::drawLine(int x0, int y0, int x1, int y1, RGB color, int thickness) {
    if (thickness <= 1) {
        drawLine(x0, y0, x1, y1, color);
        return;
    }

    float angle = atan2(y1 - y0, x1 - x0);
    float dx = sin(angle) * thickness / 2;
    float dy = cos(angle) * thickness / 2;

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        tft.drawLine(x0 + dx * i, y0 - dy * i, x1 + dx * i, y1 - dy * i, color.getValue());
    }
}

void LCD::drawLineAngle(int x0, int y0, int angle, int length, RGB color) {
    tft.drawLine(x0, y0, x0 + length * cos(angle * PI / 180), y0 + length * sin(angle * PI / 180), color.getValue());
}

void LCD::drawLineAngle(int x0, int y0, int angle, int length, RGB color, int thickness) {
    if (thickness <= 1) {
        drawLineAngle(x0, y0, angle, length, color);
        return;
    }

    float rad = angle * PI / 180;
    float dx = sin(rad) * thickness / 2;
    float dy = cos(rad) * thickness / 2;

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        tft.drawLine(x0 + dx * i, y0 - dy * i, x0 + (length * cos(rad)) + dx * i, y0 + (length * sin(rad)) - dy * i, color.getValue());
    }
}

void LCD::fillScreen(RGB color) {
    tft.fillScreen(color.getValue());
}

RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

uint16_t RGB::getValue() const {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}