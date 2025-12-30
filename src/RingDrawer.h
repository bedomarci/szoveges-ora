#ifndef RING_DRAWER_H
#define RING_DRAWER_H

#include "IDrawer.h"
#include <vector>

class RingDrawer : public IDrawer {
public:
  RingDrawer();

  // Settings
  void setAngle(float angleDegrees); // 0 = Top, 90 = Right
  void setColors(Color fg, Color bg);
  void setBlur(float blurAmount); // 0.1 to ...

  // IDrawer
  void update(uint32_t currentMillis) override;
  std::vector<Pixel> draw() override;

private:
  float currentAngle;
  Color foreground;
  Color background;
  float blur;

  // Cache or calculate on fly?
  // Let's calculate on fly for simplicity first.
};

#endif // RING_DRAWER_H
