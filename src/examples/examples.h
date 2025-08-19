#ifndef EXAMPLES_H
#define EXAMPLES_H

#define LED_PIN_1 13
#define LED_PIN_2 14
#define LED_PIN_3 15
#define LED_PIN_4 16

#define SW_PIN_1 11  // 0x5000072C
#define SW_PIN_2 12  // 0x50000730
#define SW_PIN_3 24  // 0x50000760
#define SW_PIN_4 25  // 0x50000764

int example_blinky();

// Show the current pressed switches by turning on/off the corresponding LEDs.
int example_switches_led();

// Send the current pressed switches via radio, and turn on/off the corresponding LEDs.
int example_switches_radio();

// Send the current pressed switches via radio, turn on/off the corresponding LEDs, and send
// the corresponding keypress via USB HID.
int example_switches_radio_hid();

// Send values on the radio at an interval, and show the corresponding received values with
// LEDs.
int example_radio_auto_tx();

#endif  // EXAMPLES_H
