#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "tf_card.h"


typedef struct
{
    const char *fileName;
    const char *columnSetup;

    FATFS fs;
    FIL fil;
    FRESULT fr;
    UINT bw;

    pico_fatfs_spi_config_t config;

} SDCard;


/**
 * Initialize the SD card.
 */
static inline void sd_card_init(
    SDCard *sd,
    const char *fileName,
    const char *columnSetup)
{
    sd->fileName = fileName;
    sd->columnSetup = columnSetup;

    // Configure SPI for pico_fatfs using default settings
    sd->config = (pico_fatfs_spi_config_t){
        spi0,
        CLK_SLOW_DEFAULT,
        CLK_FAST_DEFAULT,
        PIN_SPI0_MISO_DEFAULT,
        PIN_SPI0_CS_DEFAULT,
        PIN_SPI0_SCK_DEFAULT,
        PIN_SPI0_MOSI_DEFAULT,
        true    // use internal pullup
    };

    pico_fatfs_set_config(&sd->config);
}


/**
 * Mount the SD card and create/overwrite the configured file.
 *
 * Returns true on success, false on failure.
 */
static inline bool sd_card_mount(SDCard *sd)
{
    // Try mounting up to 5 times
    for (int i = 0; i < 5; i++)
    {
        sd->fr = f_mount(&sd->fs, "", 1);

        if (sd->fr == FR_OK)
        {
            break;
        }

        printf("Mount failed (%d), trying again...\n", sd->fr);

        pico_fatfs_reboot_spi();
    }

    // Failed to mount
    if (sd->fr != FR_OK)
    {
        printf("Failed to mount SD card (%d)\n", sd->fr);
        return false;
    }

    // Create/overwrite the file
    sd->fr = f_open(
        &sd->fil,
        sd->fileName,
        FA_WRITE | FA_CREATE_ALWAYS
    );

    if (sd->fr != FR_OK)
    {
        printf("Failed to open/create file (%d)\n", sd->fr);
        return false;
    }

    // Write the column/header setup
    size_t length = strlen(sd->columnSetup);

    sd->fr = f_write(
        &sd->fil,
        sd->columnSetup,
        length,
        &sd->bw
    );

    // Close file
    FRESULT close_result = f_close(&sd->fil);

    if (sd->fr != FR_OK)
    {
        printf("Failed to write column setup (%d)\n", sd->fr);
        return false;
    }

    if (close_result != FR_OK)
    {
        printf("Failed to close file (%d)\n", close_result);
        return false;
    }

    // Check that everything was written
    if (sd->bw != length)
    {
        printf(
            "Warning: only wrote %u of %u bytes\n",
            (unsigned int)sd->bw,
            (unsigned int)length
        );

        return false;
    }

    return true;
}


/**
 * Append a string to the configured file.
 *
 * Returns true on success, false on failure.
 */
static inline bool sd_card_write_string(
    SDCard *sd,
    const char *inputStr)
{
    if (inputStr == NULL)
    {
        return false;
    }

    sd->fr = f_open(
        &sd->fil,
        sd->fileName,
        FA_WRITE | FA_OPEN_APPEND
    );

    if (sd->fr != FR_OK)
    {
        printf(
            "Failed to open file for append (%d)\n",
            sd->fr
        );

        f_unmount("");

        return false;
    }

    size_t length = strlen(inputStr);

    sd->fr = f_write(
        &sd->fil,
        inputStr,
        length,
        &sd->bw
    );

    FRESULT close_result = f_close(&sd->fil);

    if (sd->fr != FR_OK)
    {
        printf(
            "Failed to write to file (%d)\n",
            sd->fr
        );

        return false;
    }

    if (close_result != FR_OK)
    {
        printf(
            "Failed to close file (%d)\n",
            close_result
        );

        return false;
    }

    // Check that everything was written
    if (sd->bw != length)
    {
        printf(
            "Warning: only wrote %u of %u bytes\n",
            (unsigned int)sd->bw,
            (unsigned int)length
        );

        return false;
    }

    return true;
}


/**
 * Unmount the SD card.
 */
static inline void sd_card_unmount(SDCard *sd)
{
    (void)sd;

    f_unmount("");
}
