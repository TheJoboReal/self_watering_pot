/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "deep_sleep.h"
#include "defines.h"
#include "soil.h"
#include "controller.h"
#include "soil_sensor.h"
#include "pump.h"
#include "eink.h"
#include "scheduler.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "time.h"
#include <stdio.h>

static struct Soil soil;

void init() {
  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(0);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  soil.moisture_level = 0;

  init_motor();

  sleep_ms(500);
  initEinkDisplay();
  sleep_ms(500);

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

void eink_task() { einkUi(&soil); }

void pump_task() { 
  soil.target_pwm = 1249;
  soil.target_water_time = 500;
  run_motor(&soil); }

// Array of tasks for the scheduler and their frequencies in milliseconds
//
// This can't be `const`, as the task values are modifies
// as the scheduler runs.
static Task TASKS[] = {
    CREATE_TASK(led_task),
    CREATE_TASK(soil_senor_task),
    CREATE_TASK(pump_task),
    CREATE_TASK(eink_task),
  
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
