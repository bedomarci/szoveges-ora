#include "LedDriver.h"
#include "board_com_api.h"
#include "tal_api.h"

// For PR_DEBUG/PR_ERR
#include "tkl_output.h"

LedDriver::LedDriver() : strip_handle(NULL) {}

bool LedDriver::init() {
  PR_DEBUG("Initializing TDL LED Strip\r\n");

  // Register hardware (if not already done in main)
  board_register_hardware();

  /* Find leds strip pixels device */
  OPERATE_RET rt = tdl_pixel_dev_find(LEDS_PIXEL_NAME, &strip_handle);
  if (rt != OPRT_OK) {
    PR_ERR("tdl_pixel_dev_find failed: %d\r\n", rt);
    return false;
  }

  /* Open leds strip pixels device */
  PIXEL_DEV_CONFIG_T pixels_cfg = {
      .pixel_num = LED_STRIP_Pixel_Count,
      .pixel_resolution = 1000, // Based on example
  };

  rt = tdl_pixel_dev_open(strip_handle, &pixels_cfg);
  if (rt != OPRT_OK) {
    PR_ERR("tdl_pixel_dev_open failed: %d\r\n", rt);
    return false;
  }

  clear();
  return true;
}

void LedDriver::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
  if (strip_handle && index < LED_STRIP_Pixel_Count) {
    PIXEL_COLOR_T color = {
        .red = (uint16_t)r,
        .green = (uint16_t)g,
        .blue = (uint16_t)b,
        .cold = 0,
        .warm = 0,
    };
    // Correct TDL function: set_single_color(handle, start_index, count, color)
    tdl_pixel_set_single_color(strip_handle, index, 1, &color);
  }
}

void LedDriver::clear() {
  if (strip_handle) {
    PIXEL_COLOR_T black = {
        .red = 0,
        .green = 0,
        .blue = 0,
        .cold = 0,
        .warm = 0,
    };
    tdl_pixel_set_single_color_all(strip_handle, &black);
    tdl_pixel_dev_refresh(strip_handle);
  }
}

void LedDriver::show() {
  if (strip_handle) {
    tdl_pixel_dev_refresh(strip_handle);
  }
}
