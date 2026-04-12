#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "humidity_reader.h"
#include <stdio.h>

#define LED_GPIO 5

void LED_task() {
  static bool led_state = false;
  gpio_set_level(LED_GPIO, led_state);
  led_state = !led_state;
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void app_main(void) {
  char *myTask = pcTaskGetName(NULL);
  ESP_LOGI(myTask, "Hello ESP32!");

  gpio_reset_pin(LED_GPIO);
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

  gpio_reset_pin(HUMIDITY_SDA_PIN);

  while (1) {
    gpio_set_level(LED_GPIO, 1);
    printf("LED ON\n");

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    gpio_set_level(LED_GPIO, 0);
    printf("LED OFF new\n");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
