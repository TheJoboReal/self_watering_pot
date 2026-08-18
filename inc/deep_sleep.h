#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#ifndef CLOCK_SOURCE
#if PICO_RP2040
#define CLOCK_SOURCE DORMANT_CLOCK_SOURCE_XOSC
#else
#define CLOCK_SOURCE DORMANT_CLOCK_SOURCE_LPOSC
#endif
#endif

#include "defines.h"
#include "pico/low_power.h"
#include "pico/stdlib.h"
#include <stdio.h>

void init_deep_sleep() { gpio_init(LOW_POWER_WAKE_GPIO); }

void deep_sleep() {
  int rc = low_power_dormant_until_gpio_pin_state(LOW_POWER_WAKE_GPIO, true,
                                                  true, CLOCK_SOURCE, NULL);
  if (rc != PICO_OK) {
    printf("Goodnight!");
    hard_assert(false);
  }
}

#endif // !DEEP_SLEEP_H
