#include "TextClock.h"
#include <cctype>
#include <iostream> /* For debug if needed */

TextClock::TextClock() : wordCount(0) {
  // Default config values are already in struct
}

void TextClock::setConfig(const DayPeriodConfig &c) { config = c; }

void TextClock::clearWords() { wordCount = 0; }

void TextClock::addWord(Word w) {
  if (wordCount < 20) {
    currentWords[wordCount++] = w;
  }
}

// Helper for Hours
Word TextClock::getNumberWord(int n) const {
  switch (n) {
  case 1:
    return EgyOra;
  case 2:
    return KettoOra;
  case 3:
    return HaromOra;
  case 4:
    return NegyOra;
  case 5:
    return OtOra;
  case 6:
    return HatOra;
  case 7:
    return HetOra;
  case 8:
    return NyolcOra;
  case 9:
    return KilencOra;
  case 10:
    return TizOra;
  case 11:
    return TizenegyOra;
  case 12:
    return TizenkettoOra;
  default:
    return EgyOra;
  }
  return EgyOra;
}

const Word *TextClock::getWords(int &count) const {
  count = wordCount;
  return currentWords;
}

void TextClock::update(int hour, int minute) {
  clearWords();

  // 1. Normalize
  if (minute > 59)
    minute = 59;
  if (minute < 0)
    minute = 0;
  while (hour >= 24)
    hour -= 24;
  while (hour < 0)
    hour += 24;

  // 2. Logic to determine Reference Hour and Period
  bool useNextHour = false;

  // Analyze minute
  int closestTargetMin = 0;
  int refPoint = 0;

  if (minute <= 8) {
    // [Hour] related
    refPoint = 0; // XX:00
    closestTargetMin = 0;
  } else if (minute <= 23) {
    // Negyed related
    refPoint = 1; // XX:15
    closestTargetMin = 15;
    useNextHour = true;
  } else if (minute <= 38) {
    // Fel related
    refPoint = 2; // XX:30
    closestTargetMin = 30;
    useNextHour = true;
  } else if (minute <= 53) {
    // Haromnegyed related
    refPoint = 3; // XX:45
    closestTargetMin = 45;
    useNextHour = true;
  } else {
    // Next Hour related
    refPoint = 4; // XX+1:00
    closestTargetMin = 60;
    useNextHour = true;
  }

  // Adjust Reference Hour for Words
  int displayedHourRaw = useNextHour ? (hour + 1) % 24 : hour;

  // Config Check
  Word periodWord;

  int h = displayedHourRaw;

  // Az eredeti idohoz kepest hatarozzuk meg, hogy melyik napszakba tartozik az
  // ido.
  //  Period Selection
  //  logic based on User Request:
  //  Midnight 00:00
  //  Night 00:01 - 03:59
  //  Dawn 04:00 - 05:59
  //  Morning 06:00 - 09:59
  //  Forenoon 10:00 - 11:59
  //  Noon 12:00
  //  Afternoon 12:01 - 17:59
  //  Evening 18:00 - 23:59

  // Note: periodWord is the "Time of Day" prefix (e.g. "Reggel" 6 ora).
  // Uses 'hour' (actual time) not 'displayedHourRaw' (rounded). user intent
  // seems to be about the actual time of day. Actually, for "Este 8 ora"
  // (20:00), we typically use the hour. But if it's 19:55 (Este 8 ora lesz 5
  // perc mulva), the period is still Este? Or does it switch to "Este" because
  // 8 is evening? User manual edit changed 'h' (displayedHourRaw) to 'hour'
  // (actual). Let's Respect User's Edit using 'hour' for range checks, but we
  // must handle the exact 00:00/12:00 cases carefully.

  if (hour == 0 && minute == 0) {
    // Midnight point - usually handled by addHourNumber if refPoint is
    // specific, but here we set periodWord which might be used if we aren't at
    // the exact point? Actually, if it is EXACTLY 00:00, we typically just say
    // "Éjfél". But let's set a fallback period.
    periodWord = Ejfel;
  } else if (hour == 12 && minute == 0) {
    periodWord = Del;
  } else {
    // Intervals
    if (hour >= 0 && hour < config.dawnStart) {
      // Night: 00:00 (covered above) to 03:59
      // (Since 00:00 is above, this effectively covers 00:01-03:59 if integer
      // logic holds, but 'hour' is just integer. 00:55 is hour 0.)
      periodWord = Ejjel;
    } else if (hour >= config.dawnStart && hour < config.morningStart) {
      periodWord = Hajnal;
    } else if (hour >= config.morningStart && hour < config.forenoonStart) {
      periodWord = Reggel;
    } else if (hour >= config.forenoonStart && hour < config.noonStart) {
      periodWord = Delelott;
    } else if (hour == 12) {
      // 12:01 - 12:59
      periodWord = Delutan;
    } else if (hour > 12 && hour < config.eveningStart) {
      periodWord = Delutan;
    } else {
      // 18:00 - 23:59
      periodWord = Este;
    }
  }

  bool nounIsSpecial = false;
  if (h == 12) {
    nounIsSpecial = true;
  }
  if (h == 0) {
    nounIsSpecial = true;
  }

  if (!nounIsSpecial) {
    addWord(periodWord);
  }

  // 1. Target Noun Phrase construction
  int hourNum = h;
  if (hourNum > 12)
    hourNum -= 12;
  if (hourNum == 0)
    hourNum = 12;

  auto addHourNumber = [&]() {
    // Only use DEL/EJFEL if referencing the exact hour (RefPoint 0 or 4)
    // For fractional times (Negyed, Fel, Haromnegyed), use Tizenketto (12).
    if ((refPoint == 0 || refPoint == 4) && h == 12)
      addWord(Del);
    else if ((refPoint == 0 || refPoint == 4) && h == 0)
      addWord(Ejfel);
    else {
      // Map number
      if (hourNum == 1)
        addWord(EgyOra);
      else if (hourNum == 2)
        addWord(KettoOra);
      else if (hourNum == 3)
        addWord(HaromOra);
      else if (hourNum == 4)
        addWord(NegyOra);
      else if (hourNum == 5)
        addWord(OtOra);
      else if (hourNum == 6)
        addWord(HatOra);
      else if (hourNum == 7)
        addWord(HetOra);
      else if (hourNum == 8)
        addWord(NyolcOra);
      else if (hourNum == 9)
        addWord(KilencOra);
      else if (hourNum == 10)
        addWord(TizOra);
      else if (hourNum == 11)
        addWord(TizenegyOra);
      else if (hourNum == 12)
        addWord(TizenkettoOra);
    }
  };

  // Construct TNP
  if (refPoint == 0 || refPoint == 4) {
    // Target is Key Hour
    addHourNumber();
  } else if (refPoint == 1) {
    // Negyed
    addWord(Negyed);
    addHourNumber();
  } else if (refPoint == 2) {
    // Fél
    addWord(Fel);
    addHourNumber();
  } else if (refPoint == 3) {
    // Háromnegyed
    addWord(Haromnegyed);
    addHourNumber();
  }

  // 2. Relation Logic
  int diff = minute - closestTargetMin;

  if (diff == 0) {
    // Exact. No suffix.
  } else if (diff > 0) {
    // Múlt
    addWord(Mult);
    int d = diff;
    // Use Perc Enums
    if (d == 1)
      addWord(EgyPerc);
    else if (d == 2)
      addWord(KettoPerc);
    else if (d == 3)
      addWord(HaromPerc);
    else if (d == 4)
      addWord(NegyPerc);
    else if (d == 5)
      addWord(OtPerc);
    else if (d == 6)
      addWord(HatPerc);
    else if (d == 7)
      addWord(HetPerc);
    else if (d == 8)
      addWord(NyolcPerc);

    addWord(Perccel);
  } else {
    // Future (Lesz ... Múlva)
    addWord(Lesz);

    int d = -diff;
    if (d == 1)
      addWord(EgyPerc);
    else if (d == 2)
      addWord(KettoPerc);
    else if (d == 3)
      addWord(HaromPerc);
    else if (d == 4)
      addWord(NegyPerc);
    else if (d == 5)
      addWord(OtPerc);
    else if (d == 6)
      addWord(HatPerc);
    else if (d == 7)
      addWord(HetPerc);
    else if (d == 8)
      addWord(NyolcPerc);

    addWord(Perc);
    addWord(Mulva);
  }
}

std::string TextClock::render() const {
  std::string s = "";
  for (int i = 0; i < wordCount; i++) {
    std::string wStr = wordToString(currentWords[i]);
    if (i == 0) {
      if (wStr.length() > 0)
        wStr[0] = toupper(wStr[0]);
    }
    s += wStr;
    if (i < wordCount - 1)
      s += " ";
  }
  s += ".";
  return s;
}

std::string TextClock::wordToString(Word w) const {
  switch (w) {
  case Del:
    return "del";
  case Ejfel:
    return "ejfel";
  case Ejjel:
    return "ejjel";
  case Hajnal:
    return "hajnal";
  case Reggel:
    return "reggel";
  case Delelott:
    return "delelott";
  case Delutan:
    return "delutan";
  case Este:
    return "este";

  case Haromnegyed:
    return "haromnegyed";
  case Negyed:
    return "negyed";
  case Fel:
    return "fel";

  case EgyOra:
    return "egy";
  case KettoOra:
    return "ketto";
  case HaromOra:
    return "harom";
  case NegyOra:
    return "negy";
  case OtOra:
    return "ot";
  case HatOra:
    return "hat";
  case HetOra:
    return "het";
  case NyolcOra:
    return "nyolc";
  case KilencOra:
    return "kilenc";
  case TizOra:
    return "tiz";
  case TizenegyOra:
    return "tizenegy";
  case TizenkettoOra:
    return "tizenketto";

  case EgyPerc:
    return "egy";
  case KettoPerc:
    return "ketto";
  case HaromPerc:
    return "harom";
  case NegyPerc:
    return "negy";
  case OtPerc:
    return "ot";
  case HatPerc:
    return "hat";
  case HetPerc:
    return "het";
  case NyolcPerc:
    return "nyolc";

  case Mult:
    return "mult";
  case Lesz:
    return "lesz";
  case Perccel:
    return "perccel";
  case Perc:
    return "perc";
  case Mulva:
    return "mulva";
  default:
    return "";
  }
}
