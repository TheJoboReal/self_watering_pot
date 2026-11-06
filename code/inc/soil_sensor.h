#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include "hardware/adc.h"
#include "soil.h"
#include <stdint.h>

void measure_moisture(struct Soil *soil) {
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = adc_read() ;
  float result_volt = result * conversion_factor;

  soil->moisture_level = result_volt;
}

#endif // SOIL_SENSOR_H
