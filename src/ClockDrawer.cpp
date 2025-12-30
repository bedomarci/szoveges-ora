#include "ClockDrawer.h"
#include <Arduino.h>

ClockDrawer::ClockDrawer() : hour(0), minute(0), offsetX(2), offsetY(2) {
  color = {0, 255, 0}; // Default Green
  initializeMappings();
}

void ClockDrawer::setTime(int h, int m) {
  hour = h;
  minute = m;
}

void ClockDrawer::setColor(Color c) { color = c; }

void ClockDrawer::setOffset(int x, int y) {
  offsetX = x;
  offsetY = y;
}

void ClockDrawer::update(uint32_t currentMillis) {
  textClock.update(hour, minute);
}

std::vector<Pixel> ClockDrawer::draw() {
  if (!isEnabled())
    return {};

  std::vector<Pixel> pixels;
  int count = 0;
  const Word *words = textClock.getWords(count);

  for (int i = 0; i < count; i++) {
    Word w = words[i];
    // Find mapping
    for (const auto &map : wordMappings) {
      if (map.word == w) {
        for (auto p : map.coords) {
          Pixel px;
          // Apply logical offset here.
          // 12x12 grid plus OffsetX/Y (default 2,2) centers it on 16x16
          px.x = p.first + offsetX;
          px.y = p.second + offsetY;
          px.r = color.r;
          px.g = color.g;
          px.b = color.b;
          pixels.push_back(px);
        }
        break;
      }
    }
  }
  return pixels;
}

void ClockDrawer::initializeMappings() {
  auto add = [&](Word w, const std::vector<std::pair<int, int>> &coords) {
    WordCoords wc;
    wc.word = w;
    wc.coords = coords;
    wordMappings.push_back(wc);
  };

  // Mappings ported from DisplayManager.cpp
  // Coordinates are Logical (0-11)

  // HAJNAL (R0 C0-5)
  add(Hajnal, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}});

  // EJJEL (R0 C6-10)
  add(Ejjel, {{6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}});

  // REGGEL (R1 C0-5)
  add(Reggel, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});

  // ESTE (Vert implies at Col 9, Rows 0-3)
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

  // TIZEN (Prefix for 11, 12)
  add(TizenegyOra, {{6, 4},
                    {7, 4},
                    {8, 4},
                    {9, 4},
                    {10, 4}, // TIZEN
                    {11, 3},
                    {11, 4},
                    {11, 5}}); // EGY (Vert)

  // TIZEN + KETTŐ
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
  add(EgyPerc, {{9, 10}, {10, 10}, {11, 10}});
  add(KettoPerc, {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}});
  add(HaromPerc, {{3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}});
  add(NegyPerc, {{8, 9}, {9, 9}, {10, 9}, {11, 9}});
  add(OtPerc, {{4, 8}, {5, 8}});
  add(HatPerc, {{5, 9}, {6, 9}, {7, 9}});
  add(HetPerc, {{0, 10}, {1, 10}, {2, 10}});
  add(NyolcPerc, {{7, 8}, {8, 8}, {9, 8}, {10, 8}, {11, 8}});
}
