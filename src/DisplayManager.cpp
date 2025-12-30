#include "DisplayManager.h"

DisplayManager::DisplayManager() { initializeMappings(); }

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

  // Time Intervals and Modifiers
  wordMappings[Hajnal] = {34, 35, 36, 37, 38, 39};
  wordMappings[Ejjel] = {40, 41, 42, 43, 44};
  wordMappings[Reggel] = {61, 60, 59, 58, 57, 56};
  wordMappings[Este] = {43, 52, 75, 84}; // Vert
  wordMappings[Delelott] = {66, 67, 68, 69, 70, 71, 72, 73};
  wordMappings[Delutan] = {66, 67, 68, 74, 75, 76, 77};
  wordMappings[Haromnegyed] = {93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83};
  wordMappings[Negyed] = {88, 87, 86, 85, 84, 83};
  wordMappings[Fel] = {100, 101, 102};
  wordMappings[Mult] = {149, 148, 147, 146};
  wordMappings[Lesz] = {162, 163, 164, 165};
  wordMappings[Perc] = {221, 220, 219, 218};
  wordMappings[Perccel] = {221, 220, 219, 218, 217, 216, 215};
  wordMappings[Mulva] = {214, 213, 212, 211, 210};

  // Hour Words (Ora)
  wordMappings[EgyOra] = {82, 109, 114}; // Vert
  wordMappings[KettoOra] = {118, 117, 116, 115, 114};
  wordMappings[HaromOra] = {137, 138, 139, 140, 141};
  wordMappings[NegyOra] = {130, 131, 132, 133};
  wordMappings[OtOra] = {103, 104};
  wordMappings[HatOra] = {134, 120, 104}; // Diag
  wordMappings[HetOra] = {134, 135, 136};
  wordMappings[NyolcOra] = {125, 124, 123, 122, 121};
  wordMappings[KilencOra] = {157, 156, 155, 154, 153, 152};
  wordMappings[TizOra] = {104, 105, 106};
  wordMappings[TizenegyOra] = {104, 105, 106, 107,
                               108, 82,  109, 114}; // TIZEN + EGY (Vert)
  wordMappings[TizenkettoOra] = {104, 105, 106, 107, 108,
                                 118, 117, 116, 115, 114}; // TIZEN + KETTO
  wordMappings[Ejfel] = {98, 99, 100, 101, 102};
  wordMappings[Del] = {66, 67, 68};

  // Minute Words (Perc)
  wordMappings[EgyPerc] = {203, 204, 205};
  wordMappings[KettoPerc] = {189, 188, 187, 186, 185};
  wordMappings[HaromPerc] = {197, 198, 199, 200, 201};
  wordMappings[NegyPerc] = {181, 180, 179, 178};
  wordMappings[OtPerc] = {166, 167};
  wordMappings[HatPerc] = {184, 183, 182};
  wordMappings[HetPerc] = {194, 195, 196};
  wordMappings[NyolcPerc] = {169, 170, 171, 172, 173};
}
