#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "lvgl/lvgl.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lv_examples/lv_tests/lv_test_group/lv_test_group.h"
#include "esp_freertos_hooks.h"

#include "drv/ili9341.h"
#include "drv/input.h"

static void lv_tick_task(void)
{
	lv_tick_inc(portTICK_RATE_MS);
}

static void user_task(void *arg)
{
	demo_create();
	//lv_test_group_1();

	while (1)
	{
		vTaskDelay(5 / portTICK_RATE_MS);
		lv_task_handler();
	}
}

void app_main()
{
	lv_init();

	display_init();
	keypad_init();

	esp_register_freertos_tick_hook(lv_tick_task);

	xTaskCreate(
		user_task,   //Task Function
		"user_task", //Task Name
		8192,		 //Stack Depth
		NULL,		 //Parameters
		1,			 //Priority
		NULL);		 //Task Handler
}