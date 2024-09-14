cmake_minimum_required(VERSION 3.12)

set(NAME stamp-round-carrier-neopixel-demo)

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(../../third-party/pi-pico-adafruit-neopixels/pico_neopixels build)

# Add your source files
add_executable(${NAME}
    src/main.cpp
)

# Include required libraries

# Link required libraries
target_link_libraries(
    ${NAME}
    pico_stdlib
    pico_neopixel
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
