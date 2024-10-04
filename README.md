# RP2040 Stamp + Round Carrier, NeoPixel Demo

This project is a demonstration of the [RP2040 Stamp](https://lectronz.com/products/rp2040-stamp) mounted on a [round carrier](https://lectronz.com/products/rp2040-stamp-round-carrier).

You can see a demonstration of this project [On my YouTube Channel](https://youtu.be/RtAe4qzLVxg?si=iUJfAAIeLWWjr16N),

## What does it do?

This repository can be use to build a handful of programs to manipulate the ring of NeoPixels on the RP2040 Stamp Round Carrier.

All programs start with the brightness of the lights at half of the maximum. This can be changed using the "user buttons" on the round carrier (these are next to the battery connector). The brightness can be adjusted to one of 16 levels. `D14` turns the brightness down, and `D15` turns the brightness up. When holding either button, the current brightness level is displayed. All lights are lit when at maximum brightness, one light is lit when at minimum brightness.

### Tri-Band

There are three coloured bands (red, green, and blue). The center at band displays the colour at the maximum intensity for the brightness level. The same colour is spread to surrounding lights, and fades based on the distance from the centre. Where more than one band overlaps, the colours are mixed.  Each band rotates around the center. Periodically, all three dots will overlap, and there will be a single white light.  Most of the time, there will be blends of constantly changing colours.

This program is packaged up as `tri-band.uf2`, which you can install using bootsel mode (see below).

### Dual-Band

There are two coloured bands.  Their colour rotates through the spectrum over time.  Each band is the polar opposite of the other, i.e. when one band is orange, the other is blue.

This program is packaged up as `dual-band.uf2`, which you can install using bootsel mode (see below).


### Solid Colour Wheel

All lights are the same colour.  The colour rotates through the spectrum over time. Each band rotates around the center at the same speed.

This program is packaged up as `colour-wheel.uf2`, which you can install using bootsel mode (see below).


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

The easiest way to reprogram the device is to enter `bootsel` mode. In `bootsel` mode, the device appears as a USB drive and will install a firmware (UF2 file) copied or dragged onto it.  It will then restart and run the uploaded code.

### Using the Reset Button

By default, the RP2040 stamp has MicroPython installed.  Pressing the reset button twice in quick sucession will enter `bootsel` mode. This project uses [the same underlying feature of the Pico SDK](https://github.com/raspberrypi/pico-sdk/blob/6500c59d704ed2bb26d44b2a0ac7151d35287fdb/src/rp2_common/pico_bootsel_via_double_reset/pico_bootsel_via_double_reset.c#L32,), and also supports pressing reset twice to enter `bootsel` mode.

### Using the `bootsel` Pin on the Stamp 

If that doesn't work for you for any reason, you can also manually enter bootsel mode using the `bootsel` pin on the underside of the stamp:

1. Power on the device.
2. Connect the `bootsel` and `ground` connections on the rear of the device (for example, using an alligator clip).
3. While `bootsel` is still connected to `ground`, press the `reset` button on the front of the device.
4. The device will stop running the current code and a USB Drive should appear.
