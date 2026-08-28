/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "deep_sleep.h"
#include "defines.h"
#include "soil.h"
#include "soil_sensor.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "scheduler.h"
#include <stdio.h>

static struct Soil soil;

void init() {
  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(0);

  gpio_init(PUMP_PIN);
  gpio_set_dir(PUMP_PIN, GPIO_OUT);
  gpio_put(PUMP_PIN, false);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  // init deep sleep wake pin
  hard_assert(status_led_init());
  gpio_init(LOW_POWER_WAKE_GPIO);
}

void led_task() {
  static bool led_state = false;
  gpio_put(PICO_DEFAULT_LED_PIN, led_state);
  led_state = !led_state;
}

void soil_senor_task() { measure_moisture(&soil); }

void pump_task() {
  gpio_put(PUMP_PIN, true);
  sleep_ms(1000);
  gpio_put(PUMP_PIN, false);
}

static Task TASKS[] = {
    // CREATE_TASK(led_task),
    CREATE_TASK(soil_senor_task),
    CREATE_TASK(pump_task),
};

int main() {
  stdio_init_all();
  init();

  Scheduler scheduler;
  Task *tasks = (Task *)TASKS;
  uint task_count = sizeof(TASKS) / sizeof(TASKS[0]);
  scheduler_init(&scheduler, tasks, task_count);

  printf("\nTASKS:\n");
  for (int i = 0; i < task_count; i++) {
    printf("    Name: %s\n", tasks[i].name);
  }

  while (true) {
    scheduler_poll(&scheduler);
    deep_sleep();
  }
}
