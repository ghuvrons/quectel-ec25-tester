/*
 * os.h
 *
 *  Created on: Dec 1, 2021
 *      Author: janoko
 */

#ifndef APP_OS_H_
#define APP_OS_H_

#include "_defs/types.h"
#include "stm32f4xx_hal.h"

#ifdef USE_FREERTOS
#include "os-freertos.h"
#endif

#ifndef OS_TYPEDEF_THREAD
#define OS_TYPEDEF_THREAD uint8_t
#endif

#ifndef OS_TYPEDEF_EVENT
#define OS_TYPEDEF_EVENT uint8_t
#endif

#ifndef OS_TYPEDEF_QUEUE
#define OS_TYPEDEF_QUEUE uint8_t
#endif

#ifndef OS_TYPEDEF_MUTEX
#define OS_TYPEDEF_MUTEX uint8_t
#endif

#define OS_WaitReady() {\
  uint32_t readyFlag;\
  OS_GlobalEventWait(OS_EVENT_READY, &readyFlag, osFlagsNoClear, osWaitForever);\
}

#define FTASK_START         0x00000001
#define FTASK_STOP          0x00000002

// Events
#define OS_EVENT_READY 1

typedef OS_TYPEDEF_THREAD   OS_Thread_t;
typedef OS_TYPEDEF_EVENT    OS_Event_t;
typedef OS_TYPEDEF_QUEUE    OS_Queue_t;
typedef OS_TYPEDEF_MUTEX    OS_Mutex_t;

extern OS_Thread_t OS_Trd_Main;
extern OS_Thread_t OS_Trd_CAN;
extern OS_Thread_t OS_Trd_Modem;
extern OS_Thread_t OS_Trd_FOTA;
#if APP
extern OS_Thread_t OS_Trd_IO;
extern OS_Thread_t OS_Trd_MQTT;
extern OS_Thread_t OS_Trd_Command;
extern OS_Thread_t OS_Trd_Remote;
extern OS_Thread_t OS_Trd_Fingerprint;
extern OS_Thread_t OS_Trd_Audio;
#endif /* APP */

extern OS_Event_t OS_Evt_Global;
extern OS_Event_t OS_Evt_CAN;
extern OS_Event_t OS_Evt_FOTA;
#if APP
extern OS_Event_t OS_Evt_IO;
extern OS_Event_t OS_Evt_MQTT;
extern OS_Event_t OS_Evt_Command;
extern OS_Event_t OS_Evt_Remote;
extern OS_Event_t OS_Evt_Fingerprint;
extern OS_Event_t OS_Evt_Audio;
#endif /* APP */

#if APP
extern OS_Queue_t OS_Queue_VehicleState;
extern OS_Queue_t OS_Queue_Command;
extern OS_Queue_t OS_Queue_IoT_Packet;
#endif /* APP */

extern OS_Mutex_t OS_Mtx_Storage;
#if APP
extern OS_Mutex_t OS_Mtx_MQTT;
extern OS_Mutex_t OS_Mtx_VehicleState;
extern OS_Mutex_t OS_Mtx_LogBuffer;
extern OS_Mutex_t OS_Mtx_Fingerprint;
#endif /* APP */

/* Macros */

#ifndef OS_GetTick
#define OS_GetTick() (HAL_GetTick())
#endif

#ifndef OS_Delay
#define OS_Delay(ms) (HAL_Delay((ms)))
#endif

#define OS_IsTimeout(firstTick, timeout) ((OS_GetTick() - (firstTick)) > (timeout))


typedef struct {
  uint32_t version;
  Timestamp_t startOn; // in seconds
} OS_HandlerTypeDef;

extern OS_HandlerTypeDef OS;

void OS_Init(void);
void OS_Services_Init(void);
extern void OS_Run(void *args);

void OS_ControlAllThreads(uint32_t flag);
uint32_t OS_Uptime(void);
void OS_Reboot(void);
void OS_Update(void);

#endif /* G_OS_H_ */
