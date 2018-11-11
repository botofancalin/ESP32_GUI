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

/* component includes */
#include <stdio.h>

/* freertos includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_freertos_hooks.h"
#include "lv_examples/lv_apps/sysmon/sysmon.h"

/* lvgl includes */
#include "iot_lvgl.h"

/* esp includes */
#include "esp_log.h"

static TimerHandle_t lvgl_tick_timer;

static void IRAM_ATTR lv_tick_task_callback(TimerHandle_t xTimer)
{
    /* Initialize a Timer for 1 ms period and
     * in its interrupt call
     * lv_tick_inc(1); */
    lv_tick_inc(1);
}

static void user_task(void *pvParameter)
{
    sysmon_create();

    while (1)
    {
        vTaskDelay(1);
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

    /* Tick interface， Initialize a Timer for 1 ms period and in its interrupt call*/
    // esp_register_freertos_tick_hook(lv_tick_task_callback);
    lvgl_tick_timer = xTimerCreate(
        "lv_tickinc_task",
        1 / portTICK_PERIOD_MS, //period time
        pdTRUE,                 //auto load
        (void *)NULL,           //timer parameter
        lv_tick_task_callback); //timer callback
    xTimerStart(lvgl_tick_timer, 0);

    /* Display interface */
    lvgl_lcd_display_init(); /*Initialize your display*/

    vTaskDelay(50 / portTICK_PERIOD_MS);

    /* Input device interface */
    lv_indev_drv_t indevdrv = lvgl_indev_init(); /*Initialize your indev*/

    vTaskDelay(50 / portTICK_PERIOD_MS); // wait for execute lv_task_handler, avoid 'error'

    lvgl_calibrate_mouse(indevdrv);

    xTaskCreatePinnedToCore(
        user_task,   //Task Function
        "user_task", //Task Name
        2048,        //Stack Depth
        NULL,        //Parameters
        1,           //Priority
        NULL,        //Task Handler
        0);          //core to run
}
