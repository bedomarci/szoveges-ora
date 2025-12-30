#include "DisplayManager.h"
#include <Arduino.h> // For Serial debug if needed
#include <utility>   // For std::pair
#include <vector>

DisplayManager::DisplayManager() { initializeMappings(); }

uint16_t DisplayManager::getIndex(uint8_t col, uint8_t row) {
  if (col >= 12 || row >= 12)
    return 0xFFFF; // Bounds check
  return addressMap[row][col];
}

void DisplayManager::getLEDIndices(const Word *words, int count,
                                   std::vector<uint16_t> &outIndices) {
  outIndices.clear();
  for (int i = 0; i < count; i++) {
    Word w = words[i];
    if (w < 60) {
      const std::vector<uint16_t> &indices = wordMappings[w];
      outIndices.insert(outIndices.end(), indices.begin(), indices.end());
    }
  }
}

void DisplayManager::initializeMappings() {
  // Clear all
  for (int i = 0; i < 60; i++)
    wordMappings[i].clear();

  // Helper lambda to make code cleaner
  auto add = [&](Word w, const std::vector<std::pair<int, int>> &coords) {
    for (auto p : coords) {
      wordMappings[w].push_back(getIndex(p.first, p.second));
    }
  };

  // Time Intervals and Modifiers
  // HAJNAL (R0 C0-5)
  add(Hajnal, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}});

  // EJJEL (R0 C6-10)
  add(Ejjel, {{6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}});

  // REGGEL (R1 C0-5)
  add(Reggel, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});

  // ESTE (Vert implies at Col 9, Rows 0-3 based on documentation/grid)
  add(Este, {{9, 0}, {9, 1}, {9, 2}, {9, 3}});

  // DELELOTT (R2 C0-7)
  add(Delelott,
      {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}});

  // DELUTAN (R2 C0-2 + C8-11)
  add(Delutan, {{0, 2}, {1, 2}, {2, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}});

  // HAROMNEGYED (R3 C0-10)
  add(Haromnegyed, {{0, 3},
                    {1, 3},
                    {2, 3},
                    {3, 3},
                    {4, 3},
                    {5, 3},
                    {6, 3},
                    {7, 3},
                    {8, 3},
                    {9, 3},
                    {10, 3}});

  // NEGYED (R3 C5-10)
  add(Negyed, {{5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}});

  // FEL (R4 C2-4)
  add(Fel, {{2, 4}, {3, 4}, {4, 4}});

  // MULT (R7 C8-11)
  add(Mult, {{8, 7}, {9, 7}, {10, 7}, {11, 7}});

  // LESZ (R8 C0-3)
  add(Lesz, {{0, 8}, {1, 8}, {2, 8}, {3, 8}});

  // PERC (R11 C0-3)
  add(Perc, {{0, 11}, {1, 11}, {2, 11}, {3, 11}});

  // PERCCEL (R11 C0-6)
  add(Perccel, {{0, 11}, {1, 11}, {2, 11}, {3, 11}, {4, 11}, {5, 11}, {6, 11}});

  // MULVA (R11 C7-11)
  add(Mulva, {{7, 11}, {8, 11}, {9, 11}, {10, 11}, {11, 11}});

  // Hour Words (Ora)

  // EGY_ORA (Vert at Col 11, Rows 3-5)
  add(EgyOra, {{11, 3}, {11, 4}, {11, 5}});

  // KETTO_ORA (R5 C6-10)
  add(KettoOra, {{6, 5}, {7, 5}, {8, 5}, {9, 5}, {10, 5}});

  // HAROM_ORA (R6 C7-11)
  add(HaromOra, {{7, 6}, {8, 6}, {9, 6}, {10, 6}, {11, 6}});

  // NEGY_ORA (R6 C0-3)
  add(NegyOra, {{0, 6}, {1, 6}, {2, 6}, {3, 6}});

  // OT_ORA (R4 C5-6)
  add(OtOra, {{5, 4}, {6, 4}});

  // HAT_ORA (Diag: R6C4, R5C5, R4C6)
  add(HatOra, {{4, 6}, {5, 5}, {6, 4}});

  // HET_ORA (R6 C4-6)
  add(HetOra, {{4, 6}, {5, 6}, {6, 6}});

  // NYOLC_ORA (R5 C0-4)
  add(NyolcOra, {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}});

  // KILENC_ORA (R7 C0-5)
  add(KilencOra, {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}});

  // TIZ_ORA (R4 C6-8)
  add(TizOra, {{6, 4}, {7, 4}, {8, 4}});

  // TIZEN (Prefix for 11, 12): T I Z E N (6..10).
  add(TizenegyOra, {{6, 4},
                    {7, 4},
                    {8, 4},
                    {9, 4},
                    {10, 4}, // TIZEN
                    {11, 3},
                    {11, 4},
                    {11, 5}}); // EGY (Vert at Col 11)

  // TIZEN (Prefix) + KETTŐ (R5 6..10?? No, KETTO_ORA is R5 C6-10)
  // Wait, KETTO_ORA is (6,5)..(10,5).
  // TizenkettoOra should use the same LEDs as TIZEN + KETTO_ORA.
  add(TizenkettoOra, {{6, 4},
                      {7, 4},
                      {8, 4},
                      {9, 4},
                      {10, 4}, // TIZEN
                      {6, 5},
                      {7, 5},
                      {8, 5},
                      {9, 5},
                      {10, 5}}); // KETTO_ORA

  // EJFEL (R4 C0-4)
  add(Ejfel, {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}});

  // DEL (R2 C0-2)
  add(Del, {{0, 2}, {1, 2}, {2, 2}});

  // Minute Words (Perc)

  // EGY_PERC (R10 C9-11)
  add(EgyPerc, {{9, 10}, {10, 10}, {11, 10}});

  // KETTO_PERC (R9 C0-4)
  add(KettoPerc, {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}});

  // HAROM_PERC (R10 C3-7)
  add(HaromPerc, {{3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}});

  // NEGY_PERC (R9 C8-11)
  add(NegyPerc, {{8, 9}, {9, 9}, {10, 9}, {11, 9}});

  // OT_PERC (R8 C4-5)
  add(OtPerc, {{4, 8}, {5, 8}});

  // HAT_PERC (R9 C5-7)
  add(HatPerc, {{5, 9}, {6, 9}, {7, 9}});

  // HET_PERC (R10 C0-2)
  add(HetPerc, {{0, 10}, {1, 10}, {2, 10}});

  // NYOLC_PERC (R8 C7-11)
  add(NyolcPerc, {{7, 8}, {8, 8}, {9, 8}, {10, 8}, {11, 8}});
}
