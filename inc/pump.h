#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define MOTOR_PWM_PIN 15

void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;

    // Clear the interrupt flag
    uint slice_num = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);
    pwm_clear_irq(slice_num);

    if (going_up) {
        ++fade;
        if (fade > 255) {
            fade = 255;
            going_up = false;
        }
    } else {
        --fade;
        if (fade < 0) {
            fade = 0;
            going_up = true;
        }
    }

    // Set PWM level
    pwm_set_gpio_level(MOTOR_PWM_PIN, fade * fade);
}

int main() {

    // Tell GPIO 15 that PWM is controlling it
    gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);

    // Find the PWM slice associated with GPIO 15
    uint slice_num = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

    // Enable PWM interrupt
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), on_pwm_wrap);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);

    // Get default PWM configuration
    pwm_config config = pwm_get_default_config();

    // Slow the PWM counter
    pwm_config_set_clkdiv(&config, 4.f);

    // Start PWM
    pwm_init(slice_num, &config, true);

    while (1) {
        tight_loop_contents();
    }
}

