#include "LedDriver.h"
#include "esp_log.h"

static const char *TAG = "LedDriver";

LedDriver::LedDriver() : strip_handle(NULL) {}

bool LedDriver::init() {
  ESP_LOGI(TAG, "Initializing LED Strip");

  led_strip_config_t strip_config = {
      .strip_gpio_num = LED_STRIP_GPIO_PIN,
      .max_leds = LED_STRIP_Pixel_Count, // at least one LED
      .led_model = LED_MODEL_WS2812,
      .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
      .flags = {
          .invert_out = false,
      }};

  led_strip_rmt_config_t rmt_config = {.clk_src = RMT_CLK_SRC_DEFAULT,
                                       .resolution_hz = 10 * 1000 * 1000,
                                       .mem_block_symbols = 0, // Default
                                       .flags = {
                                           .with_dma = false,
                                       }};

  esp_err_t err =
      led_strip_new_rmt_device(&strip_config, &rmt_config, &strip_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Install LED strip failed: %s", esp_err_to_name(err));
    return false;
  }

  clear();
  return true;
}

void LedDriver::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
  if (strip_handle && index < LED_STRIP_Pixel_Count) {
    led_strip_set_pixel(strip_handle, index, r, g, b);
  }
}

void LedDriver::clear() {
  if (strip_handle) {
    led_strip_clear(strip_handle);
  }
}

void LedDriver::show() {
  if (strip_handle) {
    led_strip_refresh(strip_handle);
  }
}
