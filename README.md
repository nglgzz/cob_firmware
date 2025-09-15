# COB Firmware

A firmware library for building computer peripherals (currently just keyboards,
but the goal is to add more) bare-metal on the nrf52840 microcontroller.

The end goal is to use this library to build multiple devices (starting with
mouse and keyboard) that connect wirelessly to a computer through one USB
dongle.

> [!CAUTION]
>
> **Use at your own risk.**
>
> This library is incomplete and the wireless functionality is not secure
> (i.e. packets for keypresses are sent in plain text).
>
> Also, this is a project I'm working on for fun on my free time. I provide no
> guarantees of stability, compatibility, or continued maintenance. I'm sharing
> it mostly as a reference implementation.

## Why?

I wanted a wireless keyboard with the lowest power consumption I could get,
while communicating with a USB dongle and having
[TMK-like](https://github.com/tmk/tmk_core) features.

I tried to configure ZMK but after failing to create some custom behavior, I gave
up. I tried to understand the Nordic SDK with the intention of having a similar
setup as the [Mitosis keyboard](https://www.reddit.com/r/MechanicalKeyboards/comments/66588f/wireless_split_qmk_mitosis/),
but didn't manage and gave up.

I realized I was missing some basic understanding of how firmware is built, so I
decided to learn about firmware development and build a basic prototype of the
whole concept: _press a key on a device, send a radio packet to a different
device, light an LED on the receiving device._

Since then the loop has been:

1. "_How hard could it be to add \_\_\_?_"
2. Spend a month or two finding out.
3. Manage to "_add_ \_\_\_" and get super excited about it.
4. Go to step 1.

## Features

| Legend | ✅ Done | 🚧 Work in Progress | 🔮 Future | ❌ Not planned |
| ------ | ------- | ------------------- | --------- | -------------- |

| Hardware features            | Status |
| ---------------------------- | ------ |
| Support for nRF52840 MCU     | ✅     |
| Support for other nRF52 MCUs | \*❌   |
| Support for any other MCUs   | ❌     |

\*I might add support for nRF52832 at some point in the future, since I have a
few lying around.

| Keyscan features              | Status |
| ----------------------------- | ------ |
| Direct GPIO scanning          | ✅     |
| Matrix scanning               | 🚧     |
| Debounce switch state changes | ✅     |

| RADIO features                         | Status |
| -------------------------------------- | ------ |
| Raw send + receive                     | ✅     |
| Acknowledgment of packets              | ✅     |
| Automatic re-send of non ACKed packets | \*✅   |
| Encryption                             | 🔮     |
| Authentication                         | 🔮     |

\* Needs testing, and can use some improvement, but it's good enough for now.

| Keymap features    | Status |
| ------------------ | ------ |
| Multiple devices   | ✅     |
| Keymaps and layers | ✅     |
| Keycode            | ✅     |
| Mod                | ✅     |
| Mod + keycode      | ✅     |
| Layer tap          | ✅     |
| Mod tap            | \*✅   |
| Mouse keys         | 🔮     |
| Buttons            | 🔮     |

\* Needs testing.

| USB features         | Status |
| -------------------- | ------ |
| Keyboard 5KRO report | ✅     |
| Mouse HID report     | \*✅   |
| Keyboard NKRO report | ❌     |

\*Need testing with an actual device to know if the report format makes sense.

## Contributing

At the moment there's a lot more I want to do than I _can_ do, so please don't
request features.

If you want to help, the thing I'd appreciate the most is feedback on my code or
tips on how to simplify the build system. This is my first C project and I
imagine there are better ways to do a lot of the things I've done here. So if
you're reading some code in the repo and think "_this is weird, I would have done
this differently_", please do create an issue and tell me about it 🙏
