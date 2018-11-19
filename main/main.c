// Copyright 2015-2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_freertos_hooks.h"

#include "iot_lvgl.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lv_examples/lv_tests/lv_test_group/lv_test_group.h"

/* Initialize a Timer for 1 ms*/
static void lv_tick_task(void)
{
    lv_tick_inc(portTICK_RATE_MS);
}

static void user_task(void *arg)
{
    demo_create();

    while (1)
    {
        vTaskDelay(5 / portTICK_RATE_MS);
        lv_task_handler();
    }
}

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main()
{
    /* Initialize LittlevGL */
    lv_init();

    /* Display interface */
    lvgl_lcd_display_init();

    esp_register_freertos_tick_hook(lv_tick_task);

    /* Input device interface */
    //lv_indev_drv_t indevdrv = lvgl_indev_init(); /*Initialize your indev*/

    /*Check touch calibration and calibrate if needed*/
    // lvgl_calibrate_mouse(indevdrv);

xTaskCreate(
    user_task,   //Task Function
    "user_task", //Task Name
    8192,        //Stack Depth
    NULL,        //Parameters
    1,           //Priority
    NULL);       //Task Handler
}
