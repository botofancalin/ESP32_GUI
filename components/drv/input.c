#include "input.h"

static int16_t enc_diff = 0;
bool pressed = false;

bool keypad_read(lv_indev_data_t *data)
{
    if (!gpio_get_level(BTN_LEFT) && !pressed)
    {
        data->enc_diff = (enc_diff - 1);
        enc_diff = 0;
        pressed = true;
    }
    
    else if (!gpio_get_level(BTN_SELECT))
    {
        data->state = LV_INDEV_STATE_PR;
    }
    
    else if (!gpio_get_level(BTN_RIGHT) && !pressed)
    {
        data->enc_diff = (enc_diff + 1);
        enc_diff = 0;
        pressed = true;
    }

    else
    {   
        data->enc_diff = enc_diff;
        data->state = LV_INDEV_STATE_REL;
        pressed = false;
    }
    
    return false;
}

void keypad_init()
{
    // for (size_t i = BTN_LEFT; i < BTN_MAX; i++)
    // {
    gpio_set_direction(BTN_LEFT, GPIO_MODE_INPUT);
    gpio_set_direction(BTN_SELECT, GPIO_MODE_INPUT);
    gpio_set_direction(BTN_RIGHT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_LEFT, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(BTN_SELECT, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(BTN_RIGHT, GPIO_PULLUP_ONLY);
    //    }

    lv_indev_drv_t keypad_drv;      /*Descriptor of an input device driver*/
    lv_indev_drv_init(&keypad_drv); /*Basic initialization*/

    keypad_drv.type = LV_INDEV_TYPE_ENCODER; /*The touchpad is pointer type device*/
    keypad_drv.read = keypad_read;          /*Library ready your touchpad via this function*/

    kp_indev = lv_indev_drv_register(&keypad_drv); /*Finally register the driver*/
}
