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
	//Init the GUI function
	demo_create();
	//lv_test_group_1();

	while (1)
	{
		//Let's update the gui every 5ms
		vTaskDelay(5 / portTICK_RATE_MS);
		lv_task_handler();
	}
}

void app_main()
{
	//Init the lvgl component
	lv_init();

	//Init the display driver
	display_init();
	//Init the keypad driver
	keypad_init();

	//Hook up the tick task to RTOS ticker
	esp_register_freertos_tick_hook(lv_tick_task);

	//Create the user interface task
	xTaskCreate(
		user_task,   //Task Function
		"user_task", //Task Name, a name just for humans
		8192,		 //Task Stack Depth (in words)
		NULL,		 //Task Parameters
		1,			 //Task Priority
		NULL);		 //Task Handler
}