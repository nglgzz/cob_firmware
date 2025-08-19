#include "leds.h"
#include "utils.h"

int blinky() {
    init_leds();

    blink_leds(500000);

    while(1) {
        blink_leds(500000);
        delay(500000);
    }
}