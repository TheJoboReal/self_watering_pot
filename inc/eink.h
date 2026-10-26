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


/**
 * Helper function to perform full refresh that works even after partial refresh mode.
 * The EPD controller enters "partial mode" when EPD_2IN9_V2_Display_Partial() is called,
 * and stays in that mode. This helper re-initializes the controller before doing a full refresh.
 */
static inline void display_full_refresh(UBYTE *image, struct Soil *soil) {
  EPD_2IN9_V2_Init();              // Re-init controller to exit partial mode
  EPD_2IN9_V2_Display_Base(image); // Full refresh with proper waveform
}


void eink_ui() {
  Paint_SelectImage(Image);
  Paint_Clear(WHITE);


  Paint_DrawString_EN(05, 05, "Plant Update", &FONT, WHITE, BLACK);
  Paint_DrawString_EN(text_x, text_y + line_spacing, "Moisture at last reading: %s\n", soil.moisture_level,
                      &FONT, WHITE, BLACK);

  // Paint_DrawString_EN(text_x, text_y + line_spacing * 2, "Pump ran at: ", &FONT,
  //                     WHITE, BLACK);

  display_full_refresh(Paint.Image);
}

// Example code for UI
// void display_relief_modes(int progress_percent, int state, bool redraw) {
//   Paint_SelectImage(Image);
//   Paint_Clear(WHITE);
//
//   // =====================================================
//   // TITLE
//   // =====================================================
//
//   switch (state) {
//   case STATE_ATTACHMENT_MODE:
//     Paint_DrawString_EN(10, 5, "Attachment Mode", &Font16, WHITE, BLACK);
//     break;
//
//   case STATE_SIT:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, WHITE, BLACK);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, WHITE,
//                         BLACK);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Sit-To-Stand", &Font12, BLACK,
//                         WHITE);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Sit-To-Stand") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, WHITE, BLACK);
//
//     // SIT SLIDER (LEFT)
//     int sit_sx = 10;
//     int sit_sy = 75;
//     Paint_DrawString_EN(sit_sx, sit_sy - 10, "Sit:", &Font12, WHITE, BLACK);
//
//     int sit_sm = 10;
//     int sit_sw = (screen_width / 3) - (sit_sm * 2);
//     int sit_sh = 12;
//     int sit_sx_pos = sit_sx;
//     int sit_sy_pos = sit_sy;
//
//     Paint_DrawRectangle(sit_sx_pos, sit_sy_pos, sit_sx_pos + sit_sw, 
//                         sit_sy_pos + sit_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int sit_fw = (sit_sw * progress_percent) / 100;
//     Paint_DrawRectangle(sit_sx_pos + 2, sit_sy_pos + 2, sit_sx_pos + sit_fw - 2,
//                         sit_sy_pos + sit_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     // STAND SLIDER (RIGHT)
//     int stand_sx = screen_width / 3 * 2;
//     Paint_DrawString_EN(stand_sx, sit_sy - 10, "Stand:", &Font12, WHITE, BLACK);
//
//     int stand_sm = 10;
//     int stand_sw = (screen_width / 3) - (stand_sm * 2);
//     int stand_sh = 12;
//     int stand_sx_pos = stand_sx;
//     int stand_sy_pos = sit_sy;
//
//     Paint_DrawRectangle(stand_sx_pos, stand_sy_pos, stand_sx_pos + stand_sw, 
//                         stand_sy_pos + stand_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int stand_fw = (stand_sw * progress_percent) / 100;
//     Paint_DrawRectangle(stand_sx_pos + 2, stand_sy_pos + 2, stand_sx_pos + stand_fw - 2,
//                         stand_sy_pos + stand_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     Paint_DrawString_EN(10, 100, "Sit down", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10 + strlen("Sit down") * Font20.Width + 10, 100,
//                         "Press Selector", &Font16, BLACK, WHITE);
//     break;
//
//   case STATE_STAND:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, WHITE, BLACK);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, WHITE,
//                         BLACK);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Sit-To-Stand", &Font12, BLACK,
//                         WHITE);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Sit-To-Stand") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, WHITE, BLACK);
//
//     // SIT SLIDER (LEFT)
//     int stand_sit_sx = 10;
//     int stand_sit_sy = 75;
//     Paint_DrawString_EN(stand_sit_sx, stand_sit_sy - 10, "Sit:", &Font12, WHITE, BLACK);
//
//     int stand_sit_sm = 10;
//     int stand_sit_sw = (screen_width / 3) - (stand_sit_sm * 2);
//     int stand_sit_sh = 12;
//     int stand_sit_sx_pos = stand_sit_sx;
//     int stand_sit_sy_pos = stand_sit_sy;
//
//     Paint_DrawRectangle(stand_sit_sx_pos, stand_sit_sy_pos, stand_sit_sx_pos + stand_sit_sw, 
//                         stand_sit_sy_pos + stand_sit_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int stand_sit_fw = (stand_sit_sw * progress_percent) / 100;
//     Paint_DrawRectangle(stand_sit_sx_pos + 2, stand_sit_sy_pos + 2, stand_sit_sx_pos + stand_sit_fw - 2,
//                         stand_sit_sy_pos + stand_sit_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     // STAND SLIDER (RIGHT)
//     int stand_stand_sx = screen_width / 3 * 2;
//     Paint_DrawString_EN(stand_stand_sx, stand_sit_sy - 10, "Stand:", &Font12, WHITE, BLACK);
//
//     int stand_stand_sm = 10;
//     int stand_stand_sw = (screen_width / 3) - (stand_stand_sm * 2);
//     int stand_stand_sh = 12;
//     int stand_stand_sx_pos = stand_stand_sx;
//     int stand_stand_sy_pos = stand_sit_sy;
//
//     Paint_DrawRectangle(stand_stand_sx_pos, stand_stand_sy_pos, stand_stand_sx_pos + stand_stand_sw, 
//                         stand_stand_sy_pos + stand_stand_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int stand_stand_fw = (stand_stand_sw * progress_percent) / 100;
//     Paint_DrawRectangle(stand_stand_sx_pos + 2, stand_stand_sy_pos + 2, stand_stand_sx_pos + stand_stand_fw - 2,
//                         stand_stand_sy_pos + stand_stand_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     Paint_DrawString_EN(10, 100, "Stand Up", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10 + strlen("Stand Up") * Font20.Width + 10, 100,
//                         "Press Selector", &Font16, BLACK, WHITE);
//     break;
//
//     case STATE_TRAINING_SIT_TO_STAND:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, WHITE, BLACK);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, WHITE,
//                         BLACK);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Sit-To-Stand", &Font12, BLACK,
//                         WHITE);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Sit-To-Stand") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, WHITE, BLACK);
//
//     // SIT SLIDER (LEFT)
//     int train_sit_sx = 10;
//     int train_sit_sy = 75;
//     Paint_DrawString_EN(train_sit_sx, train_sit_sy - 10, "Sit:", &Font12, WHITE, BLACK);
//
//     int train_sit_sm = 10;
//     int train_sit_sw = (screen_width / 3) - (train_sit_sm * 2);
//     int train_sit_sh = 12;
//     int train_sit_sx_pos = train_sit_sx;
//     int train_sit_sy_pos = train_sit_sy;
//
//     Paint_DrawRectangle(train_sit_sx_pos, train_sit_sy_pos, train_sit_sx_pos + train_sit_sw, 
//                         train_sit_sy_pos + train_sit_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int train_sit_fw = (train_sit_sw * progress_percent) / 100;
//     Paint_DrawRectangle(train_sit_sx_pos + 2, train_sit_sy_pos + 2, train_sit_sx_pos + train_sit_fw - 2,
//                         train_sit_sy_pos + train_sit_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     // STAND SLIDER (RIGHT)
//     int train_stand_sx = screen_width / 3 * 2;
//     Paint_DrawString_EN(train_stand_sx, train_sit_sy - 10, "Stand:", &Font12, WHITE, BLACK);
//
//     int train_stand_sm = 10;
//     int train_stand_sw = (screen_width / 3) - (train_stand_sm * 2);
//     int train_stand_sh = 12;
//     int train_stand_sx_pos = train_stand_sx;
//     int train_stand_sy_pos = train_sit_sy;
//
//     Paint_DrawRectangle(train_stand_sx_pos, train_stand_sy_pos, train_stand_sx_pos + train_stand_sw, 
//                         train_stand_sy_pos + train_stand_sh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int train_stand_fw = (train_stand_sw * progress_percent) / 100;
//     Paint_DrawRectangle(train_stand_sx_pos + 2, train_stand_sy_pos + 2, train_stand_sx_pos + train_stand_fw - 2,
//                         train_stand_sy_pos + train_stand_sh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     Paint_DrawString_EN(10, 100, "Training", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10 + strlen("Stand Up") * Font20.Width + 10, 100,
//                         "Press Selector", &Font16, WHITE, WHITE);
//     break;
//
//   case STATE_STAIR_WALKING:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, WHITE, BLACK);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, WHITE,
//                         BLACK);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Stair Walking", &Font12, BLACK,
//                         WHITE);
//
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Stair Walking") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, WHITE, BLACK);
//
//     // ASCEND SLIDER (LEFT)
//     int stair_asx = 10;
//     int stair_asy = 75;
//     Paint_DrawString_EN(stair_asx, stair_asy - 10, "Ascend:", &Font12, WHITE, BLACK);
//
//     int stair_asm = 10;
//     int stair_asw = (screen_width / 3) - (stair_asm * 2);
//     int stair_ash = 12;
//     int stair_asx_pos = stair_asx;
//     int stair_asy_pos = stair_asy;
//
//     Paint_DrawRectangle(stair_asx_pos, stair_asy_pos, stair_asx_pos + stair_asw, 
//                         stair_asy_pos + stair_ash, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int stair_asfw = (stair_asw * progress_percent) / 100;
//     Paint_DrawRectangle(stair_asx_pos + 2, stair_asy_pos + 2, stair_asx_pos + stair_asfw - 2,
//                         stair_asy_pos + stair_ash - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     // DESCEND SLIDER (RIGHT)
//     int stair_dsx = screen_width / 3 * 2;
//     Paint_DrawString_EN(stair_dsx, stair_asy - 10, "Descend:", &Font12, WHITE, BLACK);
//
//     int stair_dsm = 10;
//     int stair_dsw = (screen_width / 3) - (stair_dsm * 2);
//     int stair_dsh = 12;
//     int stair_dsx_pos = stair_dsx;
//     int stair_dsy_pos = stair_asy;
//
//     Paint_DrawRectangle(stair_dsx_pos, stair_dsy_pos, stair_dsx_pos + stair_dsw, 
//                         stair_dsy_pos + stair_dsh, BLACK, DOT_PIXEL_1X1, 
//                         DRAW_FILL_EMPTY);
//
//     int stair_dfw = (stair_dsw * progress_percent) / 100;
//     Paint_DrawRectangle(stair_dsx_pos + 2, stair_dsy_pos + 2, stair_dsx_pos + stair_dfw - 2,
//                         stair_dsy_pos + stair_dsh - 2, BLACK, DOT_PIXEL_1X1,
//                         DRAW_FILL_FULL);
//
//     Paint_DrawString_EN(10, 100, "Stair Walking", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10 + strlen("Stair Walking") * Font20.Width + 10, 100,
//                         "Press Selector", &Font16, BLACK, WHITE);
//     break;
//
//   case STATE_WEIGHT_RELIEF_MODE:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, BLACK,
//                         WHITE);
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Sit-To-Stand", &Font12, WHITE,
//                         BLACK);
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Sit-To-Stand") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, WHITE, BLACK);
//     break;
//
//     case STATE_TRANSFER_MODE:
//     Paint_DrawString_EN(10, 5, "Training Mode:", &Font16, BLACK, WHITE);
//     Paint_DrawString_EN(10, 10 + Font16.Height, "Walking", &Font12, WHITE,
//                         BLACK);
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width + 10,
//                         10 + Font16.Height, "Sit-To-Stand", &Font12, WHITE,
//                         BLACK);
//     Paint_DrawString_EN(10 + strlen("Walking") * Font12.Width +
//                             strlen("Sit-To-Stand") * Font12.Width + 10 * 2,
//                         10 + Font16.Height, "Transfer", &Font12, BLACK, WHITE);
//     break;
//
//   default:
//     break;
//   }
//
//   // =====================================================
//   // PROGRESS TEXT (right side)
//   // =====================================================
//
//   char str[10];
//   sprintf(str, "%d%%", progress_percent);
//
//   int text_width = strlen(str) * Font20.Width;
//   int percent_x = screen_width - text_width - 10;
//
//   Paint_DrawString_EN(percent_x, 31, str, &Font20, WHITE, BLACK);
//
//   // =====================================================
//   // REFRESH DISPLAY
//   // =====================================================
//
//   if (!redraw) {
//     display_full_refresh(Paint.Image);
//   } else {
//     EPD_Display_Partial(Paint.Image);
//   }
// }
