#ifndef SOIL_H
#define SOIL_H

// Struct to manage moisture_level and other soil related data
struct Soil {
  float moisture_level;
  int temperature;

  int target_pwm;        // Target motor pwm, defined by the controller
  int target_water_time; // Target motor run time, defined by the
                         // controller
};

#endif // !SOIL_H
