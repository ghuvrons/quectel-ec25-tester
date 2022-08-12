/*
 * def_general.h
 *
 *  Created on: Nov 22, 2021
 *      Author: janoko
 */

#ifndef DEFS_GENERAL_H_
#define DEFS_GENERAL_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#if USE_FREERTOS
#include "cmsis_os2.h"
#endif

#define STRING_SZ 30U
#define DAYS_IN_A_WEEK 7

/* Exported types
 * --------------------------------------------*/

/* Exported unions
 * --------------------------------------------*/

typedef enum
{
  CAND_FOCAN_PROGRESS = 0x01A,
  CAND_FOCAN_CRC      = 0x01B,
  CAND_FOCAN_BACKUP   = 0x01C,
  CAND_FOCAN_INIT     = 0x01D,
  CAND_FOCAN_RUN      = 0x01E,
  CAND_FOCAN_VALIDATE = 0x01F,

  CAND_VCU_SWITCH_CTL = 0x02A,
  CAND_VCU_MODE_DATA  = 0x02B,
  CAND_VCU_CLOCK      = 0x02C,

  CAND_BL_STATUS      = 0x02D,
  CAND_BL_FOCAN_TX    = 0x0FC00000,
  CAND_BL_FOCAN_RX    = 0x0FCA0000,

  CAND_VCU            = 0x7A0,
  CAND_HMI1           = 0x7A1,
  CAND_HMI2           = 0x7A2,

  CAND_DEBUG          = 0x7FF,

  CAND_MCU_SETTING    = 0x0C0,
  CAND_MCU_BEAT_WU    = 0x1026105A,
  CAND_MCU_DATA_1     = 0x10261022,
  CAND_MCU_DATA_2     = 0x10261023,

  CAND_BMS_SETTING    = 0x000001B2,
  CAND_BMS_DATA_1     = 0x0B000000,
  CAND_BMS_DATA_2     = 0x0B100000,
} CAN_ADDR;

#endif /* CONFIG_DEFS_GENERAL_H_ */
