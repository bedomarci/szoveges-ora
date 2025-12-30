#include "tal_api.h"
#include "tal_cli.h"
#include "tkl_output.h"

// Standard Includes
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

// User Includes
#include "DisplayManager.h"
#include "LedDriver.h"
#include "TextClock.h"
#include "tuya_config.h"

// Define helper macros to map legacy ESP logging to Tuya TAL logging
#define TAG "app_main"
#define ESP_LOGI(tag, fmt, ...) PR_DEBUG("[%s] " fmt "\r\n", tag, ##__VA_ARGS__)
#define ESP_LOGE(tag, fmt, ...) PR_ERR("[%s] " fmt "\r\n", tag, ##__VA_ARGS__)

// Map FreeRTOS delays to TAL
#define pdMS_TO_TICKS(ms) ms
#define vTaskDelay(t) tal_system_sleep(t)

// Tuya Thread Handle
static THREAD_HANDLE ty_app_thread = NULL;

// Application State
LedDriver ledDriver;
TextClock clockObj;
DisplayManager displayManager;

// Color Defaults
uint8_t g_red = 10;
uint8_t g_green = 10;
uint8_t g_blue = 10;

static void update_clock_display() {
  // 1. Get Real Time
  time_t now;
  struct tm timeinfo;
  time(&now);

  // Attempt to use standard localtime
  localtime_r(&now, &timeinfo);

  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;

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
    ledDriver.setPixel(idx, g_red, g_green, g_blue);
  }
  ledDriver.show();
}

static void user_main(void) {
  // Initialize TAL Logging
  tal_log_init(TAL_LOG_LEVEL_DEBUG, 1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);
  PR_DEBUG("Starting TextClock Application...\r\n");

  // Initialize LED Strip
  if (!ledDriver.init()) {
    PR_DEBUG("LED Driver Init Failed!\r\n");
  } else {
    PR_DEBUG("LED Driver Initialized\r\n");
  }

  // Main Loop
  int cnt = 0;
  while (1) {
    // Run Application Logic
    update_clock_display();

    PR_DEBUG("Heartbeat %d\r\n", cnt++);

    tal_system_sleep(100); // 10Hz update
  }
}

static void tuya_app_thread(void *arg) {
  user_main();
  tal_thread_delete(ty_app_thread);
  ty_app_thread = NULL;
}

extern "C" {
void tuya_app_main(void) {
  static char thrd_name[] = "tuya_app_main";
  THREAD_CFG_T thrd_param = {4096, 4, thrd_name};
  tal_thread_create_and_start(&ty_app_thread, NULL, NULL, tuya_app_thread, NULL,
                              &thrd_param);
}
}
