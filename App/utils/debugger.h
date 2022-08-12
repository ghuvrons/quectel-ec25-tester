/*
 * debugger.h
 *
 *  Created on: Jan 4, 2022
 *      Author: janoko
 */

#ifndef UTILS_DEBUGGER_H_
#define UTILS_DEBUGGER_H_

#include "stm32f4xx_hal.h"
#include <stdarg.h>

void DBG_Init(void);
void DBG_Printf(const char *format, ...);
void DBG_Println(const char *format, ...);
void DBG_PrintB(const uint8_t*, uint16_t);
void DBG_vPrintf(const char *format, va_list);
void DBG_vPrintln(const char *format, va_list);

#endif /* DEBUGGER_H_ */
