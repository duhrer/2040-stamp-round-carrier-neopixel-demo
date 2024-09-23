# RP2040 Stamp + Round Carrier, NeoPixel Demo

This project is a demonstration of the [RP2040 Stamp](https://lectronz.com/products/rp2040-stamp) mounted on a [round carrier](https://lectronz.com/products/rp2040-stamp-round-carrier).

You can see a demonstration of this project [On my YouTube Channel](https://youtu.be/RtAe4qzLVxg?si=iUJfAAIeLWWjr16N),

## What does it do?

In normal operation, the device models three coloured dots, each surrounded by a "band" of its own colour. One dot is solid red, another solid green, and the last solid blue. Each dot rotates around the center and varies in intensity. When two dots overlap, their colours are merged. The speed at which the dots rotate also varies over time.

Periodically, all three dots will overlap, and there will be a single white light.  Most of the time, there will be blends of constantly changing colours.

The brightness of the lights is half of the maximum by default.  This can be changed using the "user buttons" on the round carrier (these are next to the battery connector). The brightness can be adjusted to one of 16 levels. `D14` turns the brightness down, and `D15` turns the brightness up.  When holding either button, the current brightness level is displayed.  All light are lit when at maximum brightness, one light is lit when at minimum brightness.

## Building

To build this project, you'll need to have a working [build environment for the Pico C++ SDK](https://github.com/pimoroni/pimoroni-pico/blob/main/setting-up-the-pico-sdk.md).  This will also inolve downloading most of the source required by this project.

In addition, you'll need to clone the [port of the Arduino neopixel library to c++](https://github.com/martinkooij/pi-pico-adafruit-neopixels/tree/master) and (likely) adjust the relative location in `CMakeLists.txt`.

Once you've done that, you should be able to build the project using commands like the following (from the root of the repository):

```shell
mkdir build && cd build
cmake ..
make -j8
```

Adjust the last line for the number of cores you have available.  You should then end up with a range of binaries.

## Installing

### Preparing for Installation

By default, the RP2040 stamp has MicroPython installed. This supports a software reset, where pressing the reset button twice readies the stamp for reprogramming. Once you have reprogrammed the device, it no longer supports this type of reprogramming. Instead, you'll have to:

1. Power on the device.
2. Connect the `bootsel` and `ground` connections on the rear of the device.
3. Press the `reset` button on the front of the device.

### Drag and Drop

When the device is ready to be reprogrammed, you should see a USB drive for the device. Drag the compiled UF2 file onto the USB drive to install the program. You'll know this is complete when the USB drive disappears and the lights start flashing.
