# Implementation Plan - OpenTuya TextClock Migration

## Goal Description
Migrate the existing C++ `TextClock` library into a functional OpenTuya application for ESP32, supporting a 16x16 LED matrix (boustrophedon) with a 12x12 centered word grid.

## Proposed Changes

### [Component] Core Logic Extension
#### [MODIFY] [TextClock.h](file:///x:/_DEV/szoveges-ora/src/TextClock.h)
- Add `Tizen` to `Word` enum.
- Update `update` logic to handle compound words ("tizenegy", "tizenkettő") by adding both `Tizen` and the unit word to the buffer.

### [Component] Hardware Abstraction & Display
#### [NEW] [DisplayManager.h/cpp](file:///x:/_DEV/szoveges-ora/src/DisplayManager.h)
- Implement `DisplayManager` class to handle LED address mapping.
- Use the OXPLOW layout logic to convert (row, col) to LED indices.
- Include a look-up table for all `Word` enums to LED index lists.

### [Component] OpenTuya Integration
#### [NEW] [src/tuya_app_main.c](file:///x:/_DEV/szoveges-ora/src/main.c)
- Main entry point for OpenTuya (ESP-IDF style `app_main`).
- Initialize Wi-Fi and Tuya Cloud services using `tuya_iot_xxx` APIs.
- Call `TextClock` updates on a timer or time-change event.
- Sync brightness, Alarm settings, and Snooze/Reset button events via Tuya DPs.
- Implement Alarm logic (buzzer/speaker activation).

### [Component] Build Configuration
#### [MODIFY] [platformio.ini](file:///x:/_DEV/szoveges-ora/platformio.ini)
- Update environment to `framework = espidf`.
- Configure `monitor_speed`, `upload_speed`, and partition tables.
- Add scripting to clone/link TuyaOpen components if needed.

## Verification Plan

### Automated Tests
- Extend Unity tests in `test_main.cpp` to verify new `Word` enum sequences (e.g., check that 11:30 produces `Tizen`, `EgyOra`, etc.).
- Run: `pio test -e native` (manual or via pio if possible).

### Manual Verification
- Deploy to ESP32 (simulated or real hardware if available).
- Implement Tuya Cloud connectivity and parameter sync.
- Implement hardware drivers (LED matrix, buttons, speaker/buzzer).
- Verify LED layout patterns by lighting up specific words.
