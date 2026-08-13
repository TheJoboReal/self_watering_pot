#ifndef PUMP_H
#define PUMP_H

#include "defines.h"
#include "hardware/pwm.h"
#include "pico/time.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define MOTOR_PWM_PIN 15
#define MOTOR_AIN2 14
#define MOTOR_AIN1 13
#define MOTOR_STBY 12

void init_motor() {
  // Direction
  gpio_init(MOTOR_AIN1);
  gpio_set_dir(MOTOR_AIN1, GPIO_OUT);

  gpio_init(MOTOR_AIN2);
  gpio_set_dir(MOTOR_AIN2, GPIO_OUT);

  // Standby
  gpio_init(MOTOR_STBY);
  gpio_set_dir(MOTOR_STBY, GPIO_OUT);

  // Set direction
  gpio_put(MOTOR_AIN1, 1);
  gpio_put(MOTOR_AIN2, 0);

  // Enable driver
  gpio_put(MOTOR_STBY, 1);

  // PWM
  gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);

  uint slice = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

  pwm_config config = pwm_get_default_config();

  // 20 kHz PWM
  pwm_config_set_clkdiv(&config, 1.0f);
  pwm_config_set_wrap(&config, 6249);

  pwm_init(slice, &config, true);

  // Initially off
  pwm_set_gpio_level(MOTOR_PWM_PIN, 0);
}

void run_motor(uint32_t milliseconds) {
  uint slice = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

  // 100% duty cycle
  pwm_set_gpio_level(MOTOR_PWM_PIN, 6249);

  sleep_ms(milliseconds);

  // Stop motor
  pwm_set_gpio_level(MOTOR_PWM_PIN, 0);
}

#endif
