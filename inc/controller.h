#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "defines.h"
#include "soil.h"

void controller(struct Soil *soil) {
  int temp = soil->temperature;
  int moist = soil->moisture_level;
}

#endif
