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
  int dawnStart = 4;     // Hajnal
  int morningStart = 6;  // Reggel
  int forenoonStart = 9; // Délelőtt
  int noonStart = 12;    // Dél (Exact 12 is South/Noon but period follows?)
                         // Usually: Dél (12:00), Délután (12:01-17:59?)
  int afternoonStart =
      13; // Délután starts after noon? Or 12:01?
          // Standard Hungarian: Dél usually covers 12:00-12:xx or just the
          // point. Let's assume Délután starts > 12:00.
  int eveningStart = 18; // Este? User list didn't have "Este" (Evening).
                         // User list: Hajnal, Éjjel, Reggel, Délelőtt, Délután,
                         // Dél, Éjfél. Missing "Este". User might treat 18-24
                         // or similar as something else? Ah, "Éjjel" (Night) is
                         // in the list. "Hajnal" (Dawn). I will map: 00-04:
                         // Éjjel (Night) 04-06: Hajnal (Dawn) 06-09: Reggel
                         // (Morning) 09-12: Délelőtt (Forenoon) 12:00: Dél
                         // (Noon) 12-18?: Délután (Afternoon) - user didn't
                         // give "Este". Maybe "Éjjel" starts earlier? Or
                         // "Délután" goes until Night? I will use default:
                         // Night: 22-04? Or 00-04.
                         // Let's presume "Éjjel" handles the late hours too if
                         // "Este" is missing.
  int nightStart = 22; // Éjjel starts back up?
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
