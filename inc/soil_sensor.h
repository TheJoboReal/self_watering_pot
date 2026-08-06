#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include "hardware/adc.h"
#include "soil.h"
#include <stdint.h>

void measure_moisture(struct Soil *soil) {
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = adc_read();

  soil->moisture_level = result;
}

#endif // SOIL_SENSOR_H
