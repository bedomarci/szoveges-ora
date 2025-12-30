message(STATUS "Checking CONFIG_ENABLE_LED: '${CONFIG_ENABLE_LED}'")
message(STATUS "Checking CONFIG_ENABLE_LEDS_PIXEL: '${CONFIG_ENABLE_LEDS_PIXEL}'")

if (CONFIG_ENABLE_LED STREQUAL "y" OR CONFIG_ENABLE_LED OR CONFIG_ENABLE_LEDS_PIXEL STREQUAL "y" OR CONFIG_ENABLE_LEDS_PIXEL)

# APP_PATH
set(APP_PATH ${CMAKE_CURRENT_LIST_DIR})

# APP_NAME
get_filename_component(APP_NAME ${APP_PATH} NAME)

# APP_SRC
aux_source_directory(${APP_PATH}/src APP_SRC)

# APP_INC
set(APP_INC ${APP_PATH}/include)

# APP_OPTIONS
set(APP_OPTIONS "")  # -W

########################################
# Target Configure
########################################
add_library(${EXAMPLE_LIB})
message(STATUS "EXAMPLE_LIB:${APP_PATH}")

target_sources(${EXAMPLE_LIB}
    PRIVATE
        ${APP_SRC}
    )

target_include_directories(${EXAMPLE_LIB}
    PRIVATE
        ${APP_INC}
    )

target_compile_options(${EXAMPLE_LIB}
    PRIVATE
        ${APP_OPTIONS}
    )

else()
message(WARNING "LED Configs: LED='${CONFIG_ENABLE_LED}', LEDS_PIXEL='${CONFIG_ENABLE_LEDS_PIXEL}'")
message(FATAL_ERROR "led cannot work when neither CONFIG_ENABLE_LED nor CONFIG_ENABLE_LEDS_PIXEL is set to 'y'")
endif()
