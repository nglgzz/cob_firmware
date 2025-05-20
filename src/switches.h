#ifndef SWITCH_H
#define SWITCH_H

void init_switches();

// All of these pins are on port 0
#define SW_PIN_1 11  // 0x5000072C
#define SW_PIN_2 12  // 0x50000730
#define SW_PIN_3 24  // 0x50000760
#define SW_PIN_4 25  // 0x50000764

#endif  // SWITCH_H
