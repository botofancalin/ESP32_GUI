#pragma once

#include <stdint.h>
#include "lvgl.h"
#include "driver/gpio.h"

lv_indev_t *kp_indev; 

#define	BTN_LEFT    GPIO_NUM_32
#define	BTN_SELECT  GPIO_NUM_33
#define	BTN_RIGHT   GPIO_NUM_27

void keypad_init();
bool keypad_read(lv_indev_data_t *data);
