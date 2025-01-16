#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

inline uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

inline float distance(const float x, const float y, const float xx, const float yy) {
  return sqrt( pow(x - xx, 2) + pow(y - yy, 2) );
}

void smooth_move(float &current_x, float &current_y, const float &target_x, const float &target_y, const float speed = 5);

#endif