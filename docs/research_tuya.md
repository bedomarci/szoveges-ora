# TuyaOpen SDK Research & Integration Guide

## Overview
The **TuyaOpen SDK** (formerly Tuya Link SDK) is a cross-platform AIoT framework. For **ESP32**, it heavily relies on the **Espressif IoT Development Framework (ESP-IDF)**.

## Framework Requirements
1.  **ESP-IDF**: The underlying OS/framework for ESP32. TuyaOpen acts as a component/library setting on top of ESP-IDF (FreeRTOS, LwIP, etc.).
2.  **Build System**: CMake (standard for ESP-IDF).
3.  **Recommended Environment**: 
    - **PlatformIO**: Can be used with `framework = espidf`.
    - **Native ESP-IDF**: Using Espressif's tools directly.
    - **Tuya wrapper tools**: The repo uses `tos.py` for configuration, but manual integration into a standard project is possible.

## Project Structure (ESP-IDF Style)
A typical TuyaOpen ESP32 project structure:
```
project/
├── CMakeLists.txt
├── platformio.ini (if using PIO)
├── components/
│   ├── tuya_open_sdk/ (cloned from repo)
│   └── (other custom components)
├── src/
│   ├── main.c
│   └── (app logic)
└── include/
```

## Integration Steps for ESP32 (TuyaOpen)
1.  **Components**:
    - Clone `tuya/TuyaOpen` into `components/tuya_open`.
    - Clone `tuya/TuyaOpen-esp32` (platform adaptation) if required, or ensure the core SDK supports generic ESP32 targets.
    - Update `CMakeLists.txt` to register these components.

2.  **Configuration**:
    - `tuya_config.h`: Needs `PID`, `UUID`, `AUTHKEY`.
    - `menuconfig` (Kconfig): Used to enable/disable SDK features (e.g., WiFi stack adaptation).

3.  **Code Flow**:
    - `tuya_iot_init()`: Initialize SDK.
    - `tuya_iot_param_create()`: Configure connection.
    - `tuya_iot_connect()`: Connect to cloud.

## Reference Links
- [TuyaOpen GitHub](https://github.com/tuya/TuyaOpen)
- [TuyaOpen ESP32 Platform](https://github.com/tuya/TuyaOpen/tree/master/platform/esp32)
