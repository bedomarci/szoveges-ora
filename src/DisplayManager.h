#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "TextClock.h"
#include <stdint.h>
#include <vector>


class DisplayManager {
public:
  DisplayManager();

  /**
   * @brief Maps a list of Word enums to a flattened list of physical LED
   * indices.
   * @param words Pointer to word array
   * @param count Number of words
   * @param outIndices [out] Resulting LED indices
   */
  void getLEDIndices(const Word *words, int count,
                     std::vector<uint16_t> &outIndices);

private:
  struct WordMap {
    Word word;
    std::vector<uint16_t> indices;
  };

  std::vector<uint16_t>
      wordMappings[60]; // Sparse-ish array for quick lookup by enum value

  void initializeMappings();
};

#endif // DISPLAY_MANAGER_H
