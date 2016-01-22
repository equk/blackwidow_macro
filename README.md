[![License](http://img.shields.io/:license-mit-blue.svg?style=flat)](http://badges.mit-license.org)

# blackwidow_macro

    Razer BlackWidow Macro Keys Enabler
    Copyright (C) 2016 B.Walden

    This program sends a init packet used by Razer proprietary drivers
    to initialize the 'Macro' keys on Razer BlackWidow keyboards.

    License: MIT (LICENSE file should be included with source)
    
    Dependencies: libusb

## notes

BlackWidow Keyboards do not have any onboard memory for macros Razer use software for macro functionality in windows.

You can however use 'xmacro' in linux

You can also use xmodmap or your WM to allocate functions to the macro key keycodes.

## keycode reference

* M1 = 191
* M2 = 192
* M3 = 193
* M4 = 194
* M5 = 195