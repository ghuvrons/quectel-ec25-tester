/*
 * os-freertos.h
 *
 *  Created on: Jul 1, 2022
 *      Author: janoko
 */

#ifndef APP_OS_FREERTOS_H_
#define APP_OS_FREERTOS_H_


#include "cmsis_os2.h"


#define OS_TYPEDEF_THREAD osThreadId_t
#define OS_TYPEDEF_EVENT  osEventFlagsId_t
#define OS_TYPEDEF_QUEUE  osMessageQueueId_t
#define OS_TYPEDEF_MUTEX  osMutexId_t *

#define OS_WaitForever          osWaitForever
#define OS_WaitAny              osFlagsWaitAny
#define OS_WaitAll              osFlagsWaitAll
#define OS_NoClear              osFlagsNoClear
#define OS_GetTick()            (osKernelGetTickCount())
#define OS_Delay(ms)            (osDelay((ms)))
#define OS_IsFlag(notif, flag)  (((notif) & (flag)) == (flag))
#define OS_IsError(flag)        OS_IsFlag(flag, osFlagsError)

#define OS_SetTrdFlag(trdId, flag)          (osThreadFlagsSet((trdId), (flag)))
#define OS_ClearTrdFlag(flag)               (osThreadFlagsClear((flag)))
#define OS_WaitTrdFlag(flag, opt, timeout)  (osThreadFlagsWait((flag), (opt), (timeout)))
#define OS_IsFlagError(notif)               OS_IsFlag(notif, osFlagsError)

#define OS_EventWait(osEvt, reqEvt, dstEvt, opt, timeout) (*(dstEvt) = osEventFlagsWait((osEvt),(reqEvt),(opt),(timeout)))
#define OS_EventSet(osEvt, evt)                           (osEventFlagsSet((osEvt),(evt)))
#define OS_EventClear(osEvt, evt)                         (osEventFlagsClear((osEvt),(evt)))
#define OS_GlobalEventWait(reqEvt, dstEvt, opt, timeout)  (OS_EventWait(OS_Evt_Global, reqEvt, dstEvt, opt, timeout))
#define OS_GlobalEventSet(evt)                            (OS_EventSet(OS_Evt_Global, evt))

#define OS_QueueGet(mQueue, dst)                (osMessageQueueGet((mQueue), (dst), NULL, 0U))
#define OS_QueuePut(mQueue, msg)                (osMessageQueuePut((mQueue), (msg), 0U, 0U))
#define OS_QueueGetWithTO(mQueue, dst, timeout) (osMessageQueueGet((mQueue), (dst), NULL, (timeout)))
#define OS_QueuePutWithTO(mQueue, msg, timeout) (osMessageQueuePut((mQueue), (msg), 0U, (timeout)))

#define OS_MutexAcquire(mtx, to)  ((osKernelGetState() == osKernelRunning && mtx != NULL)? osMutexAcquire((mtx), (to)):osOK)
#define OS_MutexRelease(mtx)      ((osKernelGetState() == osKernelRunning && mtx != NULL)? osMutexRelease((mtx)):osOK)


#endif /* APP_OS_FREERTOS_H_ */
