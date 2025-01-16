#include "utils.h"

void smooth_move(float &current_x, float &current_y, const float &target_x, const float &target_y, const float speed) {
    float ratio, xspeed, yspeed;
    const float xdistance = abs((target_x - current_x)); 
    const float ydistance = abs((target_y - current_y));
    if (xdistance > ydistance) {
        ratio = (float)ydistance / xdistance;
        yspeed = (int)(speed * ratio);
        xspeed = speed;
    } else if (ydistance > xdistance) {
        ratio = (float)xdistance / ydistance;
        xspeed = (int)(speed * ratio);
        yspeed = speed;
    } else {
        xspeed = speed;
        yspeed = speed;
    }
    for (int i = 0; i < speed; i++) {
        if (current_x != target_x) {
            if (current_x > target_x) {
              current_x -= xspeed;
              if (current_x < target_x) {
                current_x = target_x;
              }
            }
            else if (current_x < target_x) {
              current_x += xspeed;
              if (current_x > target_x) {
                current_x = target_x;
              }
            }
        }
        if (current_y != target_y) {
            if (current_y > target_y) {
              current_y -= yspeed;
              if (current_y < target_y) {
                current_y = target_y;
              }
            }
            else if (current_y < target_y) {
              current_y += yspeed;
              if (current_y > target_y) {
                current_y = target_y;
              }
            }
        }
    }
}