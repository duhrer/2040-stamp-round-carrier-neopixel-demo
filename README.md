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

### `bootsel` Mode

The easiest way to reprogram the device is to enter `bootsel` mode. In `bootsel` mode, the device appears as a USB drive and will install any firmware (UF2 file) copied or dragged onto it.  It will then restart and run the uploaded code.

### Using the Reset Button

By default, the RP2040 stamp has MicroPython installed.  Pressing the reset button twice in quick sucession will enter `bootsel` mode. This project uses [the same underlying feature of the Pico SDK](https://github.com/raspberrypi/pico-sdk/blob/6500c59d704ed2bb26d44b2a0ac7151d35287fdb/src/rp2_common/pico_bootsel_via_double_reset/pico_bootsel_via_double_reset.c#L32,), and also supports pressing reset twice to enter `bootsel` mode.

### Using the `bootsel` Pin on the Stamp 

If that doesn't work for you for any reason, you can also manually enter bootsel mode using the `bootsel` pin on the underside of the stamp:

1. Power on the device.
2. Connect the `bootsel` and `ground` connections on the rear of the device (for example, using an alligator clip).
3. While `bootsel` is still connected to `ground`, press the `reset` button on the front of the device.
4. The device will stop running the current code and a USB Drive should appear.
