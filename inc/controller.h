#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "defines.h"
#include "soil.h"

void controller(struct Soil *soil) {
  int temp = soil->temperature;
  float moist_volt = soil->moisture_level;

  float moist_per = (3.3 - moist_volt) / (3.3 - 2.35);

}

#endif
