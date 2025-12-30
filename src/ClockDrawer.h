#ifndef CLOCK_DRAWER_H
#define CLOCK_DRAWER_H

#include "IDrawer.h"
#include "TextClock.h"
#include <vector>

class ClockDrawer : public IDrawer {
public:
  ClockDrawer();

  void setTime(int hour, int minute);
  void setColor(Color c);
  void setOffset(int x, int y);

  // IDrawer implementation
  void update(uint32_t currentMillis) override;
  std::vector<Pixel> draw() override;

private:
  TextClock textClock;
  int hour;
  int minute;
  Color color;
  int offsetX;
  int offsetY;

  struct WordCoords {
    Word word;
    std::vector<std::pair<int, int>> coords; // Logical (Col, Row) 0-11
  };

  std::vector<WordCoords> wordMappings;

  void initializeMappings();
};

#endif // CLOCK_DRAWER_H
