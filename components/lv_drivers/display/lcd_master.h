
#ifndef _LCD_MASTER_H_
#define _LCD_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../lvgl/lvgl.h"

#define WROVER 1

#ifdef WROVER
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5
#endif

#ifdef GO
#define PIN_NUM_DC 21
#define PIN_NUM_CS 5
#define PIN_NUM_BCKL 14
#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_CLK 18
#define PIN_NUM_RST 0
#endif

//To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many. More means more memory use,
//but less overhead for setting up / finishing transfers. Make sure 240 is dividable by this.
#define PARALLEL_LINES (LV_VDB_SIZE / LV_HOR_RES)

void push_image(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * data);
void start_lcd();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif