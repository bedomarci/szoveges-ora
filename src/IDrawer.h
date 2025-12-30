#ifndef IDRAWER_H
#define IDRAWER_H

#include "Types.h"
#include <vector>

class IDrawer {
public:
  virtual ~IDrawer() {}

  /**
   * @brief Update the drawer state.
   * @param currentMillis Current time in milliseconds.
   */
  virtual void update(uint32_t currentMillis) = 0;

  /**
   * @brief Draw the content.
   * @return A vector of Pixels in logical coordinates.
   */
  virtual std::vector<Pixel> draw() = 0;

  void enable(bool state) { enabled = state; }
  bool isEnabled() const { return enabled; }

protected:
  bool enabled = true;
};

#endif // IDRAWER_H
