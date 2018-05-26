# SH1106mini

This is a library for the SH1106 based 128x64 pixel OLED display running on the Arduino Attiny platform.

You can either download this library as a zip file and unpack it to your Arduino/libraries folder or (once it has been added) 
choose it from the Arduino library manager.

## Usage

The Spirit level example shows how to use the graphics capability of the library. It makes use of the TinyWireM library.

## Features

An 8 x 8 bitmap (or font) can be drawn at any valid location within the display area. Note that if the bitmap does not fall on a 
vertical 8-bit boundary then two 8-bit writes are made. This may overwrite already written locations. 
When writing strings to the display there will be corruption if lines are too close.
