#include "DisplayManager.h"
#include "LedDriver.h"
#include "TextClock.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tuya_config.h"
#include <stdio.h>


// Note: tuya headers will be added once the component is available
// #include "tuya_iot.h"

static const char *TAG = "app_main";

// Global Objects
LedDriver ledDriver;
TextClock clockObj;
DisplayManager displayManager;

static void update_clock_display() {
  // 1. Get Time (Mock for now)
  int hour = 12;
  int minute = 30; // Mock

  // 2. Logic
  clockObj.update(hour, minute);

  // 3. Map to LEDs
  int wordCount = 0;
  const Word *words = clockObj.getWords(wordCount);

  std::vector<uint16_t> pixelIndices;
  displayManager.getLEDIndices(words, wordCount, pixelIndices);

  // 4. Update Strip
  ledDriver.clear();
  for (uint16_t idx : pixelIndices) {
    ledDriver.setPixel(idx, 10, 10, 10); // Low brightness white
  }
  ledDriver.show();
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Starting TextClock Application...");

  // Initialize LED Strip
  if (!ledDriver.init()) {
    ESP_LOGE(TAG, "LED Driver Init Failed!");
  } else {
    ESP_LOGI(TAG, "LED Driver Initialized");
  }

  // Initialize Tuya (Placeholder)
  // tuya_iot_init("./");

  while (1) {
    update_clock_display();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
