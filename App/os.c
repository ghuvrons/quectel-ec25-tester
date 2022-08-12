/*
 * os.c
 *
 *  Created on: Jan 28, 2022
 *      Author: janoko
 */

#include "main.h"
#include "os.h"
#include <usart.h>
#include "os-freertos.h"
#include "utils/debugger.h"

#define DBG_BUF_TX_SZ 1024
#define DBG_BUF_RX_SZ 1024

OS_HandlerTypeDef OS;

static char *txt = "wokeeoke\r\n";
static uint8_t DBG_BufferRx[DBG_BUF_RX_SZ];

// threads
OS_Thread_t OS_Trd_Main;
OS_Thread_t OS_Trd_IO;
OS_Thread_t OS_Trd_CAN;
OS_Thread_t OS_Trd_Modem;
OS_Thread_t OS_Trd_MQTT;
OS_Thread_t OS_Trd_FOTA;
OS_Thread_t OS_Trd_Command;
OS_Thread_t OS_Trd_Remote;
OS_Thread_t OS_Trd_Fingerprint;
OS_Thread_t OS_Trd_Audio;

// events
OS_Event_t OS_Evt_Global;
OS_Event_t OS_Evt_IO;
OS_Event_t OS_Evt_CAN;
OS_Event_t OS_Evt_MQTT;
OS_Event_t OS_Evt_FOTA;
OS_Event_t OS_Evt_Command;
OS_Event_t OS_Evt_Remote;
OS_Event_t OS_Evt_Fingerprint;
OS_Event_t OS_Evt_Audio;

// sueues
OS_Queue_t OS_Queue_VehicleState;
OS_Queue_t OS_Queue_Command;
OS_Queue_t OS_Queue_IoT_Packet;

// mutexes
OS_Mutex_t OS_Mtx_MQTT;
OS_Mutex_t OS_Mtx_Storage;
OS_Mutex_t OS_Mtx_VehicleState;
OS_Mutex_t OS_Mtx_LogBuffer;
OS_Mutex_t OS_Mtx_Fingerprint;


void OS_Init(void)
{

  DBG_Println("OS Init");

  DBG_Println("OS ready");
}


__weak void OS_Services_Init(void) {}

void OS_Run(void *args)
{
  DBG_Println("OS Run");
//  HAL_UART_Transmit_DMA(&huart2, &DBG_BufferTx[0], DBG_BUF_TX_SZ);
  HAL_UART_Receive_DMA(&huart2, &DBG_BufferRx[0], DBG_BUF_RX_SZ);

  /* Infinite loop */
  for(;;)
  {

    if (HAL_UART_Transmit(&huart2, (uint8_t*) &txt[0], 10, 1000) == HAL_OK)
      OS_Delay(100);
    else OS_Delay(1000);
    HAL_GPIO_TogglePin(GPIOB, User_LED_Pin);
  }
}


void OS_ControlAllThreads(uint32_t flag)
{
  OS_EventSet(OS_Evt_CAN, flag);
  OS_EventSet(OS_Evt_Remote, flag);
  // OS_SetTrdFlag(AudioTaskHandle, flag);
}


uint32_t OS_Uptime(void)
{
  return 0;
}
