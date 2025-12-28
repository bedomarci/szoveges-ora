idf_component_register(SRCS "main.cpp" "DisplayManager.cpp" "TextClock.cpp" "LedDriver.cpp"
                        INCLUDE_DIRS "."
                        REQUIRES led_strip)
