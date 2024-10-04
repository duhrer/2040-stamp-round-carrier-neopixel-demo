cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

set(PROJECT stamp-round-carrier-neopixel-demo)

# Gooey boilerplate
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(../../third-party/pi-pico-adafruit-neopixels/pico_neopixels build)

set(TRIBAND tri-band)

# Add your source files
add_executable(${TRIBAND}
    src/tri-band.cpp
)

# Include required libraries

# Link required libraries
target_link_libraries(
    ${TRIBAND}
    pico_stdlib
    pico_neopixel
    pico_bootsel_via_double_reset
)

# By linking the reset library, we add support for entering "bootsel" mode by pressing reset twice.
# See: https://github.com/raspberrypi/pico-sdk/blob/develop/src/rp2_common/pico_bootsel_via_double_reset/pico_bootsel_via_double_reset.c#L32

# create map/bin/hex file etc.
pico_add_extra_outputs(${TRIBAND})

set(DUALBAND dual-band)

# Add your source files
add_executable(${DUALBAND}
    src/dual-band.cpp
)

# Include required libraries

# Link required libraries
target_link_libraries(
    ${DUALBAND}
    pico_stdlib
    pico_neopixel
    pico_bootsel_via_double_reset
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${DUALBAND})

set(WHEEL colour-wheel)

# Add your source files
add_executable(${WHEEL}
    src/colour-wheel.cpp
)

# Include required libraries

# Link required libraries
target_link_libraries(
    ${WHEEL}
    pico_stdlib
    pico_neopixel
    pico_bootsel_via_double_reset
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${WHEEL})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${TRIBAND}.uf2
    ${CMAKE_CURRENT_BINARY_DIR}/${DUALBAND}.uf2
    ${CMAKE_CURRENT_BINARY_DIR}/${WHEEL}.uf2    
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
