#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

struct Pixel {
  int16_t x;
  int16_t y;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#endif // TYPES_H
