#include "RingDrawer.h"
#include <Arduino.h>
#include <math.h>

RingDrawer::RingDrawer()
    : currentAngle(0), foreground({255, 0, 0}), background({0, 0, 0}),
      blur(1.0f) {}

void RingDrawer::setAngle(float angleDegrees) { currentAngle = angleDegrees; }

void RingDrawer::setColors(Color fg, Color bg) {
  foreground = fg;
  background = bg;
}

void RingDrawer::setBlur(float blurAmount) {
  blur = blurAmount;
  if (blur < 0.1f)
    blur = 0.1f;
}

void RingDrawer::update(uint32_t currentMillis) {
  // Animation logic can go here if we want smooth transitions
}

std::vector<Pixel> RingDrawer::draw() {
  if (!isEnabled())
    return {};

  std::vector<Pixel> pixels;

  // Grid Center
  float cx = 7.5f;
  float cy = 7.5f;

  // Line Vector
  // Angle 0 is UP (negative Y in graphics usually, but here Y grows down?)
  // Let's assume standard compass: 0 = Top (Y=0), 90 = Right (X=15), 180 =
  // Down, 270 = Left. In grid (0,0) is Top-Left. 0 deg (Up) -> (0, -1) vector.
  // 90 deg (Right) -> (1, 0) vector.
  // Radians
  float rad = (currentAngle - 90.0f) * PI / 180.0f; // Shift so 0 is up
  float dx = cos(rad);
  float dy = sin(rad);

  // Define Ring Area Indices logic
  // "Outer 2 rings" means x < 2 or x > 13 or y < 2 or y > 13.
  // Basically not in the 12x12 center (2..13, 2..13).

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      // Check if in Ring
      bool inRing = (x < 2 || x > 13 || y < 2 || y > 13);

      if (inRing) {
        // Pixel Center
        float px = (float)x;
        float py = (float)y;

        // Project P onto Line from C
        // Vector CP
        float vpx = px - cx;
        float vpy = py - cy;

        // Dot product magnitude
        // Line vector is unit vector L(dx, dy)
        // t = dot(CP, L)
        float t = vpx * dx + vpy * dy;

        // Closest point on infinite line
        float nx = cx + t * dx;
        float ny = cy + t * dy;

        // Distance squared from Pixel to Line
        float distSq = (px - nx) * (px - nx) + (py - ny) * (py - ny);
        float dist = sqrt(distSq);

        // Calculate intensity based on distance and angle width?
        // Wait, "pointer" is a ray. T must be positive for it to be on the ray
        // side. If t < 0, it's behind the center.

        float intensity = 0.0f;
        if (t > 0) {
          // Gaussian-ish or simple linear falloff
          // Blur factor determines width.
          // Let's say max intensity at dist=0.
          // sigma ~ blur
          intensity = exp(-(dist * dist) / (2 * blur * blur));
        }

        // Blend
        if (intensity > 0.01f) {
          Pixel p;
          p.x = x;
          p.y = y;
          p.r = (uint8_t)(background.r * (1.0f - intensity) +
                          foreground.r * intensity);
          p.g = (uint8_t)(background.g * (1.0f - intensity) +
                          foreground.g * intensity);
          p.b = (uint8_t)(background.b * (1.0f - intensity) +
                          foreground.b * intensity);
          pixels.push_back(p);
        } else {
          // Background pixel? Or transparency?
          // If we don't add it, Renderer keeps it black (or whatever was
          // there?) Renderer likely clears buffer. So we should probably return
          // BG pixels too? Or just return everything in the ring.
          Pixel p;
          p.x = x;
          p.y = y;
          p.r = background.r;
          p.g = background.g;
          p.b = background.b;
          pixels.push_back(p);
        }
      }
    }
  }

  return pixels;
}
