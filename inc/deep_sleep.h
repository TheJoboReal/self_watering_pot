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
#include "pico/status_led.h"
#include "pico/stdlib.h"
#include <stdio.h>

static uint32_t count = 1;

void deep_sleep() {
  status_led_set_state(true);

  printf("Wake up, test run: %u\n", count++);

  if (gpio_get(LOW_POWER_WAKE_GPIO)) {
      printf("Awake until gpio %u goes low\n", LOW_POWER_WAKE_GPIO);
      while(gpio_get(LOW_POWER_WAKE_GPIO)) {
          tight_loop_contents();
      }
  }

  // go dormant
  printf("Dormant until gpio %u goes high\n", LOW_POWER_WAKE_GPIO);
  status_led_set_state(false);

  int rc = low_power_dormant_until_gpio_pin_state(LOW_POWER_WAKE_GPIO, true, true, CLOCK_SOURCE, NULL);
  status_led_set_state(true);
  if (rc != PICO_OK) {
      printf("low_power_dormant_until_aon_timer returned error %d\n", rc);
      hard_assert(false);
  }
}

#endif // !DEEP_SLEEP_H
