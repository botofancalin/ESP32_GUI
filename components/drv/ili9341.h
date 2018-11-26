/**
 * @file lv_templ.h
 *
 */

#ifndef ILI9341_H
#define ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define ILI9341_HOR_RES	LV_HOR_RES
#define ILI9341_VER_RES	LV_VER_RES

#define ILI9341_MOSI 23
#define ILI9341_MISO 25
#define ILI9341_CLK  19
#define ILI9341_CS   22
#define ILI9341_DC   21
#define ILI9341_RST  18
#define ILI9341_BCKL 5
#define ILI9341_BCKL_ON 0

#define USE_SPI VSPI_HOST
#define SPI_FREQ 26

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void display_init();

void ili9431_init(void);
void ili9431_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void ili9431_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map);
void disp_spi_init(void);
void disp_spi_send(uint8_t * data, uint16_t length);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ILI9341_H*/
