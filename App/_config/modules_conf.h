/*
 * modules_conf.h
 *
 *  Created on: Dec 15, 2021
 *      Author: janoko
 */

#ifndef CONFIG_MODULES_CONF_H_
#define CONFIG_MODULES_CONF_H_

#include "stdint.h"

#if USE_FREERTOS
#include "cmsis_os2.h"
#endif

/**
 * RTC CONFIG
 */
#define RTC_ENABLE APP
/* RTC CONFIG */

/**
 * STORAGE CONFIG
 */
#define EEPROM_Delay(ms) osDelay((ms))
/* STORAGE CONFIG */


/**
 * CAN CONFIG
 */
// DRIVER
#define CAN_LISTENER_MAX 8
#define CAN_SYNC_LISTENER_MAX 8
/* CAN CONFIG */


/**
 * BMS CONFIG
 */
#define BMS_PACK_NUM 2U
/* BMS CONFIG */


/**
 * DMA_STREAMER CONFIG
 */
#if USE_FREERTOS
#define STRM_GetTick() osKernelGetTickCount()
#define STRM_Delay(ms) osDelay(ms)
#endif
/* DMA_STREAMER CONFIG */


/**
 * NET CONFIG
 */
// MODULE
// DRIVER

#include <simple-modem/config.h>
/* SIMCOM CONFIG */


/**
 * IOT CONFIG
 */
#if USE_FREERTOS
#define GIoT_GetTick() osKernelGetTickCount()
#define GIoT_Delay(ms) osDelay(ms)
#endif
/* IOT CONFIG */

/* DRIVER ADAFRUIT FINGERPRINT CONFIG */
#if USE_FREERTOS
#define AFGR_GetTick() osKernelGetTickCount()
#define AFGR_Delay(ms) osDelay(ms)
#endif


/* IO MODULE */
#define EN_GPIO_BUZZER  1
#define EN_GPIO_LED     1
#define EN_GPIO_STORAGE 1
#define EN_GPIO_NET     1
#define EN_GPIO_CAN     1
#define EN_GPIO_HMI     1
#if APP
#define EN_GPIO_SYS12V  1
#define EN_GPIO_PWR5V   1
#define EN_GPIO_HORN    1
#define EN_GPIO_MCU     1
#define EN_GPIO_COOLER  1
#define EN_GPIO_SEAT    1
#define EN_GPIO_IMU     1
#define EN_GPIO_RMT     1
#define EN_GPIO_FGR     1
#define EN_GPIO_AUDIO   1
// handle bar
#define EN_GPIO_HB_STARTER    1
#define EN_GPIO_HB_STOP       1
#define EN_GPIO_HB_LAMPBEAM   1
#define EN_GPIO_HB_SEIN       1
#define EN_GPIO_HB_ABS        1
#define EN_GPIO_HB_REVERSE    1
#define EN_GPIO_HB_SELECTSET  1 && EN_FEATURE_HB_SELECT_SET
#endif

#endif /* CONFIG_MODULES_CONF_H_ */
