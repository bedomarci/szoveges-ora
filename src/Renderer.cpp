#include "Renderer.h"
#include <Arduino.h>
#include <FastLED.h>

extern CRGB leds[]; // Defined in main.cpp, or we declare here
#ifndef NUM_LEDS
#define NUM_LEDS 256
#endif

Renderer::Renderer() { buildAddressMap(); }

void Renderer::addDrawer(IDrawer *drawer) { drawers.push_back(drawer); }

void Renderer::update(uint32_t currentMillis) {
  for (auto d : drawers) {
    d->update(currentMillis);
  }
}

void Renderer::render() {
  // Clear buffer (FastLED)
  FastLED.clear();

  // Draw each drawer
  for (auto d : drawers) {
    std::vector<Pixel> pixels = d->draw();
    for (auto p : pixels) {
      uint16_t idx = getIndex(p.x, p.y);
      if (idx < NUM_LEDS) {
        leds[idx].r += p.r;
        leds[idx].g += p.g;
        leds[idx].b += p.b;
        // Blend? Simple replace for now.
        // If we want blending, we need to read current leds[idx] and mix.
      }
    }
  }

  FastLED.show();
}

void Renderer::buildAddressMap() {
  // Fill the 16x16 address map based on the "Snake in Columns" layout
  // Column 0 (Left): Down
  // Column 1: Up
  // ...
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      int index = 0;
      if (x % 2 == 0) {
        // Even Column: Down (0 -> 15)
        index = x * 16 + y;
      } else {
        // Odd Column: Up (15 -> 0)
        index = x * 16 + (15 - y);
      }
      addressMap[y][x] = index;
    }
  }
}

uint16_t Renderer::getIndex(int x, int y) {
  if (x < 0 || x >= 16 || y < 0 || y >= 16)
    return 0xFFFF;
  return addressMap[y][x];
}
