#include "driver/gpio.h"
#include "esp_rom_sys.h" // for ets_delay_us
#include "esp_timer.h"
#include <stdio.h>

#define HUMIDITY_SDA_PIN GPIO_NUM_4

void read_humidity() {

  // --- Start signal ---
  gpio_set_direction(HUMIDITY_SDA_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(HUMIDITY_SDA_PIN, 0);
  esp_rom_delay_us(800);
  gpio_set_level(HUMIDITY_SDA_PIN, 1);

  // Switch to input
  gpio_set_direction(HUMIDITY_SDA_PIN, GPIO_MODE_INPUT);

  // --- Wait for sensor response ---
  while (gpio_get_level(HUMIDITY_SDA_PIN))
    ; // LOW
  while (!gpio_get_level(HUMIDITY_SDA_PIN))
    ; // HIGH
  while (gpio_get_level(HUMIDITY_SDA_PIN))
    ; // LOW

  uint8_t bytes[5] = {0};

  for (int i = 0; i < 40; i++) {

    while (!gpio_get_level(HUMIDITY_SDA_PIN))
      ;

    int64_t t = esp_timer_get_time();

    while (gpio_get_level(HUMIDITY_SDA_PIN))
      ;

    int64_t pulse_width = esp_timer_get_time() - t;

    int bit = (pulse_width > 40) ? 1 : 0;

    bytes[i / 8] <<= 1;
    bytes[i / 8] |= bit;
  }

  uint16_t humidity = (bytes[0] << 8) | bytes[1];
  uint16_t temperature = (bytes[2] << 8) | bytes[3];
  uint8_t checksum = bytes[4];

  if ((uint8_t)(bytes[0] + bytes[1] + bytes[2] + bytes[3]) != checksum) {
    printf("Checksum error\n");
    return;
  }

  float humidity_real = humidity / 10.0f;
  float temperature_real = temperature / 10.0f;

  if (temperature & 0x8000) {
    temperature &= 0x7FFF;
    temperature_real = -(temperature / 10.0f);
  }

  printf("Humidity: %.1f%%, Temperature: %.1f°C\n", humidity_real,
         temperature_real);
}
