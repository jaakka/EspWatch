#ifndef LCD_H
#define LCD_H

#include <TFT_eSPI.h>

class RGB {
public:
    RGB(uint8_t r, uint8_t g, uint8_t b);
    uint16_t getValue() const;

private:
    uint8_t r, g, b;
};

class LCD {
public:
    LCD();
    void init();
    void drawText(String text, int x, int y, int size, RGB color);
    void drawBox(int x, int y, int w, int h, RGB color);
    void drawCircle(int x, int y, int r, RGB color);
    void drawLine(int x0, int y0, int x1, int y1, RGB color);
    void drawLine(int x0, int y0, int x1, int y1, RGB color, int thickness);
    void drawLineAngle(int x0, int y0, int angle, int length, RGB color);
    void drawLineAngle(int x0, int y0, int angle, int length, RGB color, int thickness);
    void fillScreen(RGB color);

private:
    TFT_eSPI tft;
};

#endif