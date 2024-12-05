#include "touch.h"
#include <CST816S.h>
#include <Arduino.h>

#define TOUCH_SDA 21
#define TOUCH_SCL 22
#define TOUCH_RST 4
#define TOUCH_INT 19

TOUCH::TOUCH() : obj(CST816S(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT)) {
    Serial.println("TOUCH init");
    obj.begin();
}