#include <Arduino.h>
#include <CommandParser.h>
#include <FastLED.h>
#include <TaskScheduler.h>
#include <vector>

// Include project headers
#include "ClockDrawer.h"
#include "Renderer.h"
#include "RingDrawer.h"

// Hardware Config
#define LED_PIN 2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 256 // 16x16 matrix
#define BRIGHTNESS 30

CRGB leds[NUM_LEDS];

// Architecture Components
Renderer renderer;
ClockDrawer clockDrawer;
RingDrawer ringDrawer;

typedef CommandParser<> MyCommandParser;
MyCommandParser parser;

// Scheduler
Scheduler runner;

// Forward declarations
void rendererCallback();
void serialCallback();
void sweepCallback();

// Tasks
Task tRenderer(50, TASK_FOREVER, &rendererCallback, &runner,
               false); // Disabled by default until on?
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

      // Update Drawers
      clockDrawer.setTime(g_hour, g_minute);
      // Maybe set ring angle based on minute?
      // Minute hand: 360 * minute / 60
      float angle = 360.0f * m / 60.0f;
      ringDrawer.setAngle(angle);

      tSweep.disable();
      tRenderer.enable(); // Enable renderer

      sprintf(response, "Time set to %02d:%02d", g_hour, g_minute);
      return;
    }
  }
  sprintf(response, "Invalid format. Use HHMM.");
}

// Ring Demo
void ringDemoCallback();
Task tRingDemo(50, TASK_FOREVER, &ringDemoCallback, &runner, false);

// ... (other tasks)

void setRing(MyCommandParser::Argument *args, char *response) {
  // Command: ring [angle] or [on/off]
  String arg = args[0].asString;

  if (arg.equalsIgnoreCase("on")) {
    tRingDemo.enable();
    ringDrawer.enable(true); // Ensure ring is visible
    // Keep clock enabled for composition
    sprintf(response, "Ring demo ON");
  } else if (arg.equalsIgnoreCase("off")) {
    tRingDemo.disable();
    ringDrawer.enable(false); // Hide ring
    sprintf(response, "Ring demo OFF");
  } else if (arg.length() > 0) {
    tRingDemo.disable(); // Manual control overrides demo
    float angle = arg.toFloat();
    ringDrawer.setAngle(angle);
    sprintf(response, "Ring angle set to %.1f", angle);
  } else {
    sprintf(response, "Usage: ring <angle> | on | off");
  }

  tSweep.disable();
  tRenderer.enable();
}

void ringDemoCallback() {
  // 1 lap in 60 seconds (60000ms)
  // angle = (millis % 60000) / 60000.0 * 360.0
  uint32_t ms = millis();
  float angle = (float)(ms % 60000) / 60000.0f * 360.0f;
  ringDrawer.setAngle(angle);
}

void startSweep(MyCommandParser::Argument *args, char *response) {
  tRenderer.disable(); // Disable renderer so sweep can take over
  tSweep.enable();
  sprintf(response, "Starting LED sweep test...");
}

void turnOff(MyCommandParser::Argument *args, char *response) {
  tRenderer.disable();
  tSweep.disable();
  FastLED.clear();
  FastLED.show();
  sprintf(response, "Display off.");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nTextClock Arduino (New Arch)");
  Serial.println("Commands: HHMM, t HHMM, sweep, off, ring <angle>");

  // Register commands
  parser.registerCommand("t", "s", setTime);
  parser.registerCommand("time", "s", setTime);
  parser.registerCommand("sweep", "", startSweep);
  parser.registerCommand("off", "", turnOff);
  parser.registerCommand("ring", "s", setRing);

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // Setup Architecture
  clockDrawer.setColor({0, 255, 0}); // Green
  clockDrawer.setTime(g_hour, g_minute);

  ringDrawer.setColors({255, 0, 0}, {0, 0, 0}); // Red FG
  ringDrawer.setAngle(0);
  ringDrawer.setBlur(0.6f);

  renderer.addDrawer(&clockDrawer);
  renderer.addDrawer(&ringDrawer);
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

            clockDrawer.setTime(g_hour, g_minute);
            // float angle = 360.0f * m / 60.0f;
            // ringDrawer.setAngle(angle);

            tSweep.disable();
            tRenderer.enable();
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

void rendererCallback() {
  uint32_t now = millis();
  renderer.update(now);
  renderer.render();
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
      // Don't auto-enable renderer here, stay off until command?
      Serial.println("Sweep complete.");
    }
  }
  FastLED.show();
}
