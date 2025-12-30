#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#ifndef OUT
#define OUT
#endif
#ifndef IN
#define IN
#endif
#ifndef INOUT
#define INOUT
#endif

#include "tal_api.h"
#include "tdl_pixel_color_manage.h"
#include "tdl_pixel_dev_manage.h"
#include "tuya_kconfig.h" // Include generated config macros
#include <stdint.h>


// Fallback for LED pixel name if Kconfig macro is not available
#ifndef LEDS_PIXEL_NAME
#ifdef CONFIG_LEDS_PIXEL_NAME
#define LEDS_PIXEL_NAME CONFIG_LEDS_PIXEL_NAME
#else
#define LEDS_PIXEL_NAME "led_pixel"
#endif
#endif

#define LED_STRIP_Pixel_Count 256 // 16x16

class LedDriver {
public:
  LedDriver();
  bool init();
  void setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void clear();
  void show();

private:
  PIXEL_HANDLE_T strip_handle;
};

#endif // LED_DRIVER_H
