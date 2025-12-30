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

  uint16_t addressMap[12][12] = {
      {34, 61, 66, 93, 98, 125, 130, 157, 162, 189, 194, 221},
      {35, 60, 67, 92, 99, 124, 131, 156, 163, 188, 195, 220},
      {36, 59, 68, 91, 100, 123, 132, 155, 164, 187, 196, 219},
      {37, 58, 69, 90, 101, 122, 133, 154, 165, 186, 197, 218},
      {38, 57, 70, 89, 102, 121, 134, 153, 166, 185, 198, 217},
      {39, 56, 71, 88, 103, 120, 135, 152, 167, 184, 199, 216},
      {40, 55, 72, 87, 104, 119, 136, 151, 168, 183, 200, 215},
      {41, 54, 73, 86, 105, 118, 137, 150, 169, 182, 201, 214},
      {42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 202, 213},
      {43, 52, 75, 84, 107, 116, 139, 148, 171, 180, 203, 212},
      {44, 51, 76, 83, 108, 115, 140, 147, 172, 179, 204, 211},
      {45, 50, 77, 82, 109, 114, 141, 146, 173, 178, 205, 210}
  }; // Physical address map
  uint16_t getIndex(uint8_t col,
                    uint8_t row); // Get physical index from logical coords

  std::vector<uint16_t>
      wordMappings[60]; // Sparse-ish array for quick lookup by enum value

  void initializeMappings();
};

#endif // DISPLAY_MANAGER_H
