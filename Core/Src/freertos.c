/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <os.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for OS_Thread */
osThreadId_t OS_ThreadHandle;
uint32_t OS_ThreadBuffer[ 512 ];
osStaticThreadDef_t OS_ThreadControlBlock;
const osThreadAttr_t OS_Thread_attributes = {
  .name = "OS_Thread",
  .cb_mem = &OS_ThreadControlBlock,
  .cb_size = sizeof(OS_ThreadControlBlock),
  .stack_mem = &OS_ThreadBuffer[0],
  .stack_size = sizeof(OS_ThreadBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Modem_Thread */
osThreadId_t Modem_ThreadHandle;
uint32_t Modem_ThreadBuffer[ 1024 ];
osStaticThreadDef_t Modem_ThreadControlBlock;
const osThreadAttr_t Modem_Thread_attributes = {
  .name = "Modem_Thread",
  .cb_mem = &Modem_ThreadControlBlock,
  .cb_size = sizeof(Modem_ThreadControlBlock),
  .stack_mem = &Modem_ThreadBuffer[0],
  .stack_size = sizeof(Modem_ThreadBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MQTT_Thread */
osThreadId_t MQTT_ThreadHandle;
uint32_t MQTT_ThreadBuffer[ 1024 ];
osStaticThreadDef_t MQTT_ThreadControlBlock;
const osThreadAttr_t MQTT_Thread_attributes = {
  .name = "MQTT_Thread",
  .cb_mem = &MQTT_ThreadControlBlock,
  .cb_size = sizeof(MQTT_ThreadControlBlock),
  .stack_mem = &MQTT_ThreadBuffer[0],
  .stack_size = sizeof(MQTT_ThreadBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ModemMutex */
osMutexId_t ModemMutexHandle;
osStaticMutexDef_t ModemMutexControlBlock;
const osMutexAttr_t ModemMutex_attributes = {
  .name = "ModemMutex",
  .cb_mem = &ModemMutexControlBlock,
  .cb_size = sizeof(ModemMutexControlBlock),
};
/* Definitions for MqttMutex */
osMutexId_t MqttMutexHandle;
osStaticMutexDef_t MqttMutexControlBlock;
const osMutexAttr_t MqttMutex_attributes = {
  .name = "MqttMutex",
  .cb_mem = &MqttMutexControlBlock,
  .cb_size = sizeof(MqttMutexControlBlock),
};
/* Definitions for FreeRTOS_Event_MQTT */
osEventFlagsId_t FreeRTOS_Event_MQTTHandle;
osStaticEventGroupDef_t FreeRTOS_Event_MQTTControlBlock;
const osEventFlagsAttr_t FreeRTOS_Event_MQTT_attributes = {
  .name = "FreeRTOS_Event_MQTT",
  .cb_mem = &FreeRTOS_Event_MQTTControlBlock,
  .cb_size = sizeof(FreeRTOS_Event_MQTTControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void OS_Run(void *argument);
extern void MDM_Run(void *argument);
extern void MQTT_Run(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of ModemMutex */
  ModemMutexHandle = osMutexNew(&ModemMutex_attributes);

  /* creation of MqttMutex */
  MqttMutexHandle = osMutexNew(&MqttMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of OS_Thread */
  OS_ThreadHandle = osThreadNew(OS_Run, NULL, &OS_Thread_attributes);

  /* creation of Modem_Thread */
  Modem_ThreadHandle = osThreadNew(MDM_Run, NULL, &Modem_Thread_attributes);

  /* creation of MQTT_Thread */
  MQTT_ThreadHandle = osThreadNew(MQTT_Run, NULL, &MQTT_Thread_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of FreeRTOS_Event_MQTT */
  FreeRTOS_Event_MQTTHandle = osEventFlagsNew(&FreeRTOS_Event_MQTT_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  OS_Services_Init();
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_OS_Run */
/**
  * @brief  Function implementing the OS_Thread thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_OS_Run */
__weak void OS_Run(void *argument)
{
  /* USER CODE BEGIN OS_Run */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OS_Run */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

