#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "defines.h"
#include "soil.h"

void controller(struct Soil *soil)
{
    float moist_volt = soil->moisture_level;

    /*
     * Convert sensor voltage to normalized moisture percentage.
     *
     * 3.3V  = dry
     * 2.35V = wet
     */
    float moist_per =
        (3.3f - moist_volt) / (3.3f - 2.35f);

    // Clamp to 0-100%
    if (moist_per < 0.0f)
        moist_per = 0.0f;

    if (moist_per > 100.0f)
        moist_per = 100.0f;


    /*
     * -------------------------
     * Moisture membership
     * -------------------------
     *
     * LOW:
     *   0-40%  = fully low
     *   40-60% = transitions to medium
     *
     * MEDIUM:
     *   40-60% = transitions up
     *   60-80% = transitions down
     *
     * HIGH:
     *   60-80% = transitions up
     *   80-100% = fully high
     */

    float low = 0.0f;
    float medium = 0.0f;
    float high = 0.0f;


    // LOW moisture
    if (moist_per <= 40.0f) {
        low = 1.0f;
    }
    else if (moist_per < 60.0f) {
        low = (60.0f - moist_per) / 20.0f;
    }


    // MEDIUM moisture
    if (moist_per >= 40.0f && moist_per <= 60.0f) {
        medium = (moist_per - 40.0f) / 20.0f;
    }
    else if (moist_per > 60.0f && moist_per < 80.0f) {
        medium = (80.0f - moist_per) / 20.0f;
    }


    // HIGH moisture
    if (moist_per >= 80.0f) {
        high = 1.0f;
    }
    else if (moist_per > 60.0f) {
        high = (moist_per - 60.0f) / 20.0f;
    }


    /*
     * -------------------------
     * Fuzzy rules
     * -------------------------
     *
     * LOW moisture    -> HIGH water
     * MEDIUM moisture -> MEDIUM water
     * HIGH moisture   -> NO water
     */

    float no_water = high;
    float medium_water = medium;
    float high_water = low;


    /*
     * -------------------------
     * Defuzzification
     * -------------------------
     *
     * Output values:
     *
     * NO WATER     = 0
     * MEDIUM WATER = 0.5
     * HIGH WATER   = 1.0
     */

    float numerator =
          no_water * 0.0f
        + medium_water * 0.5f
        + high_water * 1.0f;

    float denominator =
          no_water
        + medium_water
        + high_water;

    float water_level = 0.0f;

    if (denominator > 0.0f) {
        water_level = numerator / denominator;
    }


    /*
     * -------------------------
     * Water duration
     * -------------------------
     *
     * 0    -> 0 ms
     * 0.5  -> 5000 ms
     * 1.0  -> 10000 ms
     *
     * These are starting values.
     */

    int water_time =
        (int)(water_level * 10000.0f);


    soil->target_water_time = water_time;
}

#endif
