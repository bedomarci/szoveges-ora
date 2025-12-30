#include <Arduino.h>
#include <CommandParser.h>
#include <FastLED.h>
#include <TaskScheduler.h>
#include <vector>

// Include project headers
#include "DisplayManager.h"
#include "TextClock.h"

// Hardware Config
#define LED_PIN 2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 256 // 16x16 matrix
#define BRIGHTNESS 10

CRGB leds[NUM_LEDS];
TextClock textClock;
DisplayManager displayManager;

typedef CommandParser<> MyCommandParser;
MyCommandParser parser;

// Scheduler
Scheduler runner;

// Forward declarations
void clockCallback();
void serialCallback();
void sweepCallback();

// Tasks
Task tClock(1000, TASK_FOREVER, &clockCallback, &runner, false);
Task tSerial(50, TASK_FOREVER, &serialCallback, &runner, true);
Task tSweep(30, TASK_FOREVER, &sweepCallback, &runner, false);

// Global State
int g_hour = 12;
int g_minute = 0;

void setTime(MyCommandParser::Argument *args, char *response) {
  String timeStr = args[0].asString;
  if (timeStr.length() == 4) {
    int h = timeStr.substring(0, 2).toInt();
    int m = timeStr.substring(2, 4).toInt();
    if (h >= 0 && h <= 23 && m >= 0 && m <= 59) {
      g_hour = h;
      g_minute = m;
      tSweep.disable();
      tClock.enable();
      sprintf(response, "Time set to %02d:%02d", g_hour, g_minute);
      return;
    }
  }
  sprintf(response, "Invalid format. Use HHMM.");
}

void startSweep(MyCommandParser::Argument *args, char *response) {
  tClock.disable();
  tSweep.enable();
  sprintf(response, "Starting LED sweep test...");
}

void turnOff(MyCommandParser::Argument *args, char *response) {
  tClock.disable();
  tSweep.disable();
  FastLED.clear();
  FastLED.show();
  sprintf(response, "Display off.");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nTextClock Arduino Test (TaskScheduler)");
  Serial.println("Commands: HHMM, t HHMM, sweep, off");

  // Register commands
  parser.registerCommand("t", "s", setTime);
  parser.registerCommand("time", "s", setTime);
  parser.registerCommand("sweep", "", startSweep);
  parser.registerCommand("off", "", turnOff);

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() { runner.execute(); }

void serialCallback() {
  static char buffer[128];
  static size_t pos = 0;

  while (Serial.available()) {
    byte b = Serial.read();
    if (b == '\r' || b == '\n') {
      if (pos > 0) {
        buffer[pos] = '\0';
        String input = String(buffer);
        input.trim();

        char response[128];
        if (parser.processCommand(buffer, response)) {
          Serial.println(response);
        } else if (input.length() == 4 && isDigit(input[0])) {
          int h = input.substring(0, 2).toInt();
          int m = input.substring(2, 4).toInt();
          if (h >= 0 && h <= 23 && m >= 0 && m <= 59) {
            g_hour = h;
            g_minute = m;
            tSweep.disable();
            tClock.enable();
            Serial.printf("Shorthand update: %02d:%02d\n", g_hour, g_minute);
          }
        }
        pos = 0;
      }
    } else if (pos < sizeof(buffer) - 1) {
      buffer[pos++] = b;
    }
  }
}

void clockCallback() {
  textClock.update(g_hour, g_minute);
  int count = 0;
  const Word *words = textClock.getWords(count);

  std::vector<uint16_t> indices;
  displayManager.getLEDIndices(words, count, indices);

  FastLED.clear();
  for (uint16_t idx : indices) {
    if (idx < NUM_LEDS) {
      leds[idx] = CRGB::Orange;
    }
  }
  FastLED.show();
}

void sweepCallback() {
  static int step = 0;
  static bool filling = true;

  if (filling) {
    leds[step] = CRGB::Red;
    step++;
    if (step >= NUM_LEDS) {
      filling = false;
      step = 0;
      Serial.println("Panel filled. Now clearing...");
    }
  } else {
    leds[step] = CRGB::Black;
    step++;
    if (step >= NUM_LEDS) {
      filling = true;
      step = 0;
      tSweep.disable();
      tClock.enable();
      Serial.println("Sweep complete.");
    }
  }
  FastLED.show();
}
