#ifndef LED_DRIVER_H
#define LED_DRIVER_H

// #include "led_strip_encoder.h"
#include "led_strip.h"
#include <stdint.h>


// Hardware Configuration
#define LED_STRIP_GPIO_PIN 2      // Adjust as per hardware
#define LED_STRIP_Pixel_Count 256 // 16x16

class LedDriver {
public:
  LedDriver();
  bool init();
  void setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void clear();
  void show();

private:
  led_strip_handle_t strip_handle;
};

#endif // LED_DRIVER_H
