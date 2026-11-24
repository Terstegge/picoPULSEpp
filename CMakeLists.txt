cmake_minimum_required(VERSION 3.20)

# Set the target board

set(YAHAL_BOARD "rpi-pico")
# set(YAHAL_BOARD "rpi-pico2")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(picoPULSE++)

add_executable(picoPULSE++)

# Optimize application for speed
target_compile_options(picoPULSE++
        PRIVATE -Ofast
)

# Add YAHAL
yahal_add_me(picoPULSE++)
# Set YAHAL system clock to 200MHz for RP2040
if (RP2040)
target_compile_definitions(YAHAL_picoPULSE++
        PRIVATE SYS_CLK_FREQ=200
)
endif()
# Optimize YAHAL for speed
target_compile_options(YAHAL_picoPULSE++
        PRIVATE -Ofast
)


# Add USB library
yahal_add_library(picoPULSE++ tinyUSB++)
target_compile_definitions(tinyUSB++_picoPULSE++
        INTERFACE TUPP_MAX_INTERF_PER_CONF=10
        INTERFACE TUPP_MAX_STRINGS=20
)

yahal_add_custom_targets(picoPULSE++)
yahal_add_extra_outputs(picoPULSE++)

add_subdirectory(config)
add_subdirectory(src)
