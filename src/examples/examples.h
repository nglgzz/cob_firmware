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
int example_switches_led();

#endif  // EXAMPLES_H
