#ifndef LCD_H
#define LCD_H

#include <TFT_eSPI.h>

class LCD {
public:
    LCD();
    void drawText(String text, int x, int y, int size, int color);
    void drawBox(int x, int y, int w, int h, int color);
    void drawCircle(int x, int y, int r, int color);
    void drawLine(int x0, int y0, int x1, int y1, int color);
    void fillScreen(int color);

private:
    TFT_eSPI tft;
};

#endif