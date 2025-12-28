# PRD - Hungarian Text Clock (Szöveges Óra)

## Overview
A smart wall clock that displays time using Hungarian words on a 16x16 addressable LED matrix. The clock is powered by an ESP32 and integrates with the Tuya Cloud for time synchronization and configuration.

## Hardware Specifications
- **Microcontroller**: ESP32
- **Display**: 16x16 Addressable LED Matrix (WS2812B or similar)
  - **Layout**: OXPLOW / Boustrophedon (alternating row directions)
  - **Usage**: 12x12 central area used for the word grid.
- **Controls**:
  - 1x Reset Button
  - 1x Snooze Button
- **Audio**:
  - 1x Speaker (Passive buzzer or I2S DAC/Speaker for alarms)

## Software Requirements
- **Framework**: TuyaOpen SDK (based on ESP-IDF)
- **Build System**: ESP-IDF (CMake) via PlatformIO or native tools.
- **Components**:
  - `espressif/led_strip`: For WS2812B control.
- **Connectivity**: Tuya Cloud integration via Wi-Fi.
- **Features**:
  - Time synchronization via Tuya Cloud.
  - Configuration of clock parameters through the Tuya Smart App.
  - Dynamic Hungarian text generation (re-using and extending the `TextClock` library).
  - **Alarm Clock**: Configurable via App, with Snooze support.

## Word Matrix Layout (12x12 Middle)
The word grid is a 12x12 matrix containing Hungarian time-related words. Each LED in the 12x12 area represents one character.

### Word Mapping Strategy
- Each word enum will map to a list of LED addresses.
- Words can overlap (e.g., "DÉLELŐTTUTÁN").
- Compound words (e.g., "tizenegy") will be composed of multiple segments.

## OpenTuya Implementation Plan
1. **Initialize OpenTuya SDK**: Set up the project structure using the TuyaOpen framework.
2. **Tuya Cloud Integration**: Define Data Points (DPs) for time settings, brightness, and other configurations.
3. **LED Driver**: Implement a driver for the WS2812B matrix with OXPLOW layout support.
4. **Logic Porting**: Move the `TextClock` logic into the Tuya app loop, triggering updates based on NTP/Tuya time.
5. **Hardware Integration**: Map buttons to reset and snooze functionalities.

## Documentation Requirements
- Detailed research on `tuyaopen` SDK for ESP32.
- LED address lookup table for all Hungarian words.
