#ifndef TEXTCLOCK_H
#define TEXTCLOCK_H

#include <string>
#include <vector>

// Enumeration for allowed words
enum Word {
  Del,
  Ejfel,

  Ejjel, // Converted accents to ASCII for enum names usually, but user might
         // want matches. I'll use simple names.
  Hajnal,
  Reggel,
  Delelott,
  Delutan,
  Este,

  Haromnegyed,
  Negyed,
  Fel,

  EgyOra,
  KettoOra,
  HaromOra,
  NegyOra,
  OtOra,
  HatOra,
  HetOra,
  NyolcOra,
  KilencOra,
  TizOra,
  TizenegyOra,
  TizenkettoOra,

  EgyPerc,
  KettoPerc,
  HaromPerc,
  NegyPerc,
  OtPerc,
  HatPerc,
  HetPerc,
  NyolcPerc,

  // Suffixes
  Mult,
  Lesz,
  Perccel,
  Perc,
  Mulva
};

// Configuration for day periods
struct DayPeriodConfig {
  int dawnStart = 4;       // Hajnal: 04:00
  int morningStart = 6;    // Reggel: 06:00
  int forenoonStart = 10;  // Délelőtt: 10:00
  int noonStart = 12;      // Dél: 12:00
  int afterNoonStart = 12; // Délután: > 12:00
  int eveningStart = 18;   // Este: 18:00
  int nightStart = 0;      // Éjjel: 00:00 - 04:00 (and 00:00 point)
                           // Note: Éjfél is exact 00:00.
};

class TextClock {
public:
  TextClock();

  // Configuration
  void setConfig(const DayPeriodConfig &config);

  // Core Logic
  void update(int hour, int minute);

  // Display/Access
  std::string render() const;
  const Word *getWords(int &count) const;

private:
  DayPeriodConfig config;
  Word currentWords[20]; // Fixed buffer for Arduino friendliness
  int wordCount;

  // Helper to clear buffer
  void clearWords();
  // Helper to add word
  void addWord(Word w);

  // Helper to stringify enum
  std::string wordToString(Word w) const;

  // Helper to map number to Enum (0-23 or 0-59)
  // Note: User allowed words are limited.
  // Hour: 1-12 (plus variants).
  // Minute: 1-9 (1-8 used mostly).
  Word getNumberWord(int n) const;
};

#endif // TEXTCLOCK_H
