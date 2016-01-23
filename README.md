[![License](http://img.shields.io/:license-mit-blue.svg?style=flat)](http://badges.mit-license.org)

# blackwidow_macro

Razer BlackWidow Macro Keys Enabler

Copyright (C) 2016 B.Walden

This program sends a init packet used by Razer proprietary drivers to initialize the 'Macro' keys on Razer BlackWidow keyboards.

License: MIT (LICENSE file should be included with source)

Dependencies: [libusb](http://libusb.info/)

## program usage

    Razer BlackWidow Macro Keys v1
    Razer BlackWidow Device Found
    Sending data:
     00 00 00 00 00 02 00 04
     02 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     00 00 00 00 00 00 00 00
     04 00
    Transmitted: 90

## notes

BlackWidow Keyboards do not have any onboard memory for macros Razer use software for macro functionality in windows.

You can however use 'xmacro' in linux

You can also use xmodmap or your WM to allocate functions to the macro key keycodes.

To find all connected Razer devices you can use: `lsusb -d 1532:`

## extending

I have added a sample udev rule to udev/99-bwidow.rules which can be placed into /usr/lib/udev/rules.d/99-bwidow.rules

To install this you can do `make cpudev`

To remove you can do `make rmudev`

## keycode reference

* M1 = 191
* M2 = 192
* M3 = 193
* M4 = 194
* M5 = 195

## todo

* extract all known device id's of blackwidow keyboards from the Razer proprietary drivers
* add more blackwidow device id's
