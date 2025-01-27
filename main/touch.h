#ifndef TOUCH_H
#define TOUCH_H

#include <CST816S.h>

struct TouchData {
    bool available;
    uint8_t gestureID;
};

class TOUCH {
public:
    TOUCH();
    void init();
    void loop();
    bool userSwipeRight();
    bool userSwipeLeft();
    bool userSwipeUp();
    bool userSwipeDown();
    bool userTouch();
    bool userReleaseTouch();
    int x;
    int y;

private:
    CST816S obj;
    TouchData touchData;
};

#endif
