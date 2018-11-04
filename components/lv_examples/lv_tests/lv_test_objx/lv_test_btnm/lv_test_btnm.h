/**
 * @file lv_test_btnm.h
 *
 */

#ifndef LV_TEST_BTNM_H
#define LV_TEST_BTNM_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "../../../../lvgl/lvgl.h"
#include "../../../lv_ex_conf.h"
#endif

#if USE_LV_BTNM && USE_LV_TESTS

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create button matrixes to test their functionalities
 */
void lv_test_btnm_1(void);

/**********************
 *      MACROS
 **********************/

#endif /*USE_LV_BTNM*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* USE_LV_BTNM && USE_LV_TESTS*/
