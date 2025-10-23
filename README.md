# blackwidow_macro

Razer BlackWidow Macro Keys Enabler

Copyright (C) 2016 B.Walden

This program sends a init packet used by Razer proprietary drivers to initialize the 'Macro' keys on Razer BlackWidow keyboards.

License: MIT (LICENSE file should be included with source)

Dependencies: [libusb](http://libusb.info/)

## Usage

```sh
Razer BlackWidow Macro Keys v2.0
Sending Keyboard Macro Init Sequence for [1532:011b]

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
```

```sh
Arguments:
        -s      send init packet
        -v      show verbose output
```

## Version 2.x

improvements:

- [x] new libusb handle
- [x] array of keyboard device IDs
- [x] new cli feedback
- [x] support for more keyboard devices

## Notes

BlackWidow Keyboards do not have any onboard memory for macros Razer use software for macro functionality in windows.

You can however use `xmacro` in linux

You can also use xmodmap or your WM to allocate functions to the macro key keycodes.

To find all connected Razer devices you can use: `lsusb -d 1532:`

## Extending

I have added a sample udev rule to `udev/99-bwidow.rules` which can be placed into `/usr/lib/udev/rules.d/99-bwidow.rules`

To install this you can do `make cpudev`

To remove you can do `make rmudev`

### Keycode Reference

    M1 = 191
    M2 = 192
    M3 = 193
    M4 = 194
    M5 = 195

### Devices Included

    010D = "Razer BlackWidow Ultimate"
    010E = "Razer BlackWidow"
    011B = "Razer BlackWidow 2013"
    0203 = "Razer BlackWidow Chroma"
    0221 = "Razer BlackWidow Chroma V2"

If anyone wants me to add any more please tell me so I can add them in.

### Related References

* [Wireshark USB Capture Setup](https://wiki.wireshark.org/CaptureSetup/USB)
* [libusb on github](https://github.com/libusb/libusb)
* [libusb wiki on github](https://github.com/libusb/libusb/wiki)

### Razer Windows Drivers

Device IDs taken from Razer BlackWidow drivers

    Razer010D.DeviceDesc     = "Razer BlackWidow Ultimate"
    Razer010E.DeviceDesc     = "Razer BlackWidow"
    Razer011B.DeviceDesc     = "Razer BlackWidow 2013"

    %Razer010D.DeviceDesc%=Razer, HID\Vid_1532&Pid_010D&MI_00
    %Razer010E.DeviceDesc%=Razer, HID\Vid_1532&Pid_010E&MI_00
    %Razer011B.DeviceDesc%=Razer, HID\Vid_1532&Pid_011B&MI_00

# Contact

Website: https://equk.co.uk

Mastodon: [@equilibriumuk@hachyderm.io](https://hachyderm.io/@equilibriumuk)

Bluesky: [@equilibriumuk.bsky.social](https://bsky.app/profile/equilibriumuk.bsky.social)

# License

MIT License
