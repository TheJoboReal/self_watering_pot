#ifndef PUMP_H
#define PUMP_H

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "defines.h"


void init_pwm() {
    // Tell GPIO 15 that PWM is controlling it
    gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);

    // Find the PWM slice associated with GPIO 15
    uint slice_num = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

    // Get default PWM configuration
    pwm_config config = pwm_get_default_config();

    // PWM clock divider
    pwm_config_set_clkdiv(&config, 4.f);

    // Start PWM
    pwm_init(slice_num, &config, true);

    // Motor initially OFF
    pwm_set_gpio_level(MOTOR_PWM_PIN, 0);
}

void run_motor() {
    int milliseconds = 500;

    // 50% duty cycle
    // With the default 16-bit PWM range, 32768 = ~50%
    pwm_set_gpio_level(MOTOR_PWM_PIN, 32768);

    // Run motor for requested duration
    sleep_ms(milliseconds);

    // Turn motor OFF
    pwm_set_gpio_level(MOTOR_PWM_PIN, 0);
}

#endif // !PUMP_H
