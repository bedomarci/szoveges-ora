#include "DisplayManager.h"
#include "LedDriver.h"
#include "TextClock.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tuya_config.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>


#include "cJSON.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_iot.h"


static const char *TAG = "app_main";

// Tuya Objects
tuya_iot_client_t client;
tuya_iot_license_t license = {
    .uuid = TUYA_OPENSDK_UUID,
    .authkey = TUYA_OPENSDK_AUTHKEY,
};

// Application State
LedDriver ledDriver;
TextClock clockObj;
DisplayManager displayManager;

// Color/Brightness Defaults
uint8_t g_red = 10;
uint8_t g_green = 10;
uint8_t g_blue = 10;

// DP Definitions (Customize based on Tuya IoT Platform)
#define DPID_SWITCH 1
#define DPID_COLOR 2  // Hex string or similar? Simplified for now.
#define DPID_BRIGHT 3 // Value

void user_event_handler_on(tuya_iot_client_t *client, tuya_event_msg_t *event) {
  ESP_LOGI(TAG, "Tuya Event ID:%d", event->id);
  switch (event->id) {
  case TUYA_EVENT_BIND_START:
    ESP_LOGI(TAG, "Bind Start");
    break;
  case TUYA_EVENT_MQTT_CONNECTED:
    ESP_LOGI(TAG, "MQTT Connected");
    break;
  case TUYA_EVENT_TIMESTAMP_SYNC:
    ESP_LOGI(TAG, "Sync timestamp:%d", event->value.asInteger);
    // Set System Time
    struct timeval tv;
    tv.tv_sec = event->value.asInteger;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    break;
  case TUYA_EVENT_DP_RECEIVE_OBJ: {
    dp_obj_recv_t *dpobj = event->value.dpobj;
    for (int i = 0; i < dpobj->dpscnt; i++) {
      dp_obj_t *dp = dpobj->dps + i;
      ESP_LOGI(TAG, "DP ID:%d Type:%d", dp->id, dp->type);
      switch (dp->id) {
      case DPID_SWITCH:
        // Handle Switch
        break;
      case DPID_BRIGHT:
        if (dp->type == PROP_VALUE) {
          // Scale brightness 0-255 or 0-100
          float scale = dp->value.dp_value / 100.0f;
          // Simple mock scaling of global color
          // In real app, store base color and brightness separate
        }
        break;
      }
    }
    // Report state back (Shadow)
    tuya_iot_dp_obj_report(client, dpobj->devid, dpobj->dps, dpobj->dpscnt, 0);
  } break;
  default:
    break;
  }
}

static void update_clock_display() {
  // 1. Get Real Time
  time_t now;
  struct tm timeinfo;
  time(&now);
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

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Starting TextClock Application...");

  // Initialize LED Strip
  if (!ledDriver.init()) {
    ESP_LOGE(TAG, "LED Driver Init Failed!");
  } else {
    ESP_LOGI(TAG, "LED Driver Initialized");
  }

  // Initialize Tuya
  tuya_iot_config_t config = {
      .software_ver = "1.0.0",
      .productkey = TUYA_PRODUCT_ID,
      .uuid = license.uuid,
      .authkey = license.authkey,
      .event_handler = user_event_handler_on,
  };

  int rt = tuya_iot_init(&client, &config);
  if (rt != OPRT_OK) {
    ESP_LOGE(TAG, "Tuya Init Failed: %d", rt);
  } else {
    ESP_LOGI(TAG, "Tuya Init Success");
    tuya_iot_start(&client);
  }

  while (1) {
    tuya_iot_yield(&client);
    update_clock_display();
    vTaskDelay(pdMS_TO_TICKS(100)); // 10Hz update
  }
}
