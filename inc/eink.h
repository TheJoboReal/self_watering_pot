#include "../lib/Fonts/fonts.h"
#include "GUI_Paint.h"
#include "defines.h"
#include "soil.h"
#include <stdlib.h>
#include <string.h>

#include "epd.h"

#define FONT Font16
#define ENCODER_COUNTS_PER_STEP 4
#define PERCENT_PER_STEP 1

static UBYTE *Image = NULL;
static UWORD Imagesize;

// Layout
volatile int screen_width = 296;
volatile int screen_height = 128;
volatile int text_x = 3;
volatile int text_y = 15;
volatile int line_spacing = 30;

/**
 * @brief Initialize the e-ink display.
 *
 * This function initializes the e-ink display, allocates memory for the image
 * buffer, and writes RoboBooster to the display.
 *
 */
//
static int initEinkDisplay(void) {

  if (DEV_Module_Init() != 0)
    return -1;

  EPD_Init();

  Imagesize = ((EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1)) *
              EPD_HEIGHT;
  Imagesize = ((EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1)) *
              EPD_HEIGHT;

  Image = malloc(Imagesize);

  Paint_NewImage(Image, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);

  Paint_SelectImage(Image);
  Paint_Clear(WHITE);
  EPD_2IN9_V2_Display(Image);

  return 0;
}




void einkUi(struct Soil *soil) {
  Paint_SelectImage(Image);
  Paint_Clear(WHITE);

  char moisture[64];
  char time[64];

  snprintf(moisture, sizeof(moisture),
      "Moisture at last reading: %d", soil->moisture_level);
  snprintf(time, sizeof(time),
      "Time watering at last reading: %d", soil->target_water_time);

  Paint_DrawString_EN(text_x, text_y + line_spacing,
      moisture, &FONT, WHITE, BLACK);
  Paint_DrawString_EN(text_x, text_y + line_spacing*2,
      time, &FONT, WHITE, BLACK);

  EPD_2IN9_V2_Init();              // Re-init controller
  EPD_2IN9_V2_Display_Base(Image); // Full refresh
}

