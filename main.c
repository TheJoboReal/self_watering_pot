/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "defines.h"
#include "soil.h"
#include "controller.h"
#include "soil_sensor.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pump.h"
#include "scheduler.h"
#include "tusb.h"
#include <stdio.h>

static bool led_state = false;
static struct Soil soil;

void init() {
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(ADC_PIN);
  // Select ADC input 0 (GPIO26)
  adc_select_input(0);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  soil.moisture_level = 0;

  init_motor();
}

void led_task() {
  gpio_put(PICO_DEFAULT_LED_PIN, led_state);
  led_state = !led_state;
}

void serial_task() { printf("%d\n", (int)(soil.moisture_level * 10)); }

void soil_senor_task() { measure_moisture(&soil); }

void pump_task() { run_motor(); }

// Array of tasks for the scheduler and their frequencies in milliseconds
//
// This can't be `const`, as the task values are modifies
// as the scheduler runs.
static Task TASKS[] = {
    CREATE_TASK(led_task, 200),
    CREATE_TASK(soil_senor_task, 500),
    CREATE_TASK(serial_task, 500),
    CREATE_TASK(pump_task, 500),
};

int main() {
  stdio_init_all();
  tusb_init();
  init();
  sleep_ms(500);

  Scheduler scheduler;
  Task *tasks = (Task *)TASKS;
  uint task_count = sizeof(TASKS) / sizeof(TASKS[0]);
  scheduler_init(&scheduler, tasks, task_count);

  printf("\nTASKS:\n");
  for (int i = 0; i < task_count; i++) {
    printf("    Name: %s, Freq: %lld ms\n", tasks[i].name,
           tasks[i].frequency_us / 1000);
  }

  while (true) {
    tud_task();
    scheduler_poll(&scheduler);
  }
}
