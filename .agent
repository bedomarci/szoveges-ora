# Antigravity Agent Instructions - TextClock Project

This file contains critical project context for any AI agent working on the `szoveges-ora` codebase.

## Project Goal
Develop a Hungarian Text Clock using an ESP32 and OpenTuya, displaying time on a 16x16 LED matrix (12x12 active area).

## Technical Stack
- **Languages**: C++, C
- **Platform**: ESP32
- **Framework**: TuyaOpen / OpenTuya SDK
- **Test Framework**: Unity (for logic testing)

## Hardware Details
- Matrix: 16x16 Addressable LED
- Layout: OXPLOW (Boustrophedon)
- Active Area: (2,2) to (13,13) in 0-indexed 16x16 matrix (assumed centered).
- Buttons: Reset, Snooze
- Audio: Speaker/Buzzer for Alarm

## Key Files
- `src/TextClock.h/cpp`: Core logic for Hungarian time to word conversion.
- `docs/PRD.md`: Project Requirements Document.
- `instructions.md`: Original user instructions.

## Design Patterns
- Logic is separated from display hardware to allow testing on native environments (Unity).
- Tuya Cloud integration handles time sync and app-based configuration.

## Word Matrix Data
The word grid is 12 rows by 12 columns. 
Mapping must account for the reversing row directions of the OXPLOW layout.
- Even Rows (0, 2, ...): Left to Right
- Odd Rows (1, 3, ...): Right to Left
*(Relative to the start of the matrix, which might be top-left or bottom-left depending on physical wiring. Standard is top-left.)*
