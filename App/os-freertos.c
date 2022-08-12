/*
 * os-freertos.c
 *
 *  Created on: Jul 1, 2022
 *      Author: janoko
 */

#include "os-freertos.h"
#include "os.h"


extern osThreadId_t MainTaskHandle;
extern osThreadId_t CanTaskHandle;
extern osThreadId_t ModemTaskHandle;
#if APP
extern osThreadId_t IOTaskHandle;
extern osThreadId_t MqttTaskHandle;
extern osThreadId_t CommandTaskHandle;
extern osThreadId_t RemoteTaskHandle;
extern osThreadId_t AudioTaskHandle;
extern osThreadId_t FingerprintTaskHandle;
#else
extern osThreadId_t FotaTaskHandle;
#endif /* app */

#if APP
extern osMessageQueueId_t BikeStateQueueHandle;
extern osMessageQueueId_t CommandQueueHandle;
extern osMessageQueueId_t IoT_PacketQueueHandle;
#endif /* app */

extern osMutexId_t EepromMutexHandle;
#if APP
extern osMutexId_t MqttMutexHandle;
extern osMutexId_t VehicleStateMutexHandle;
extern osMutexId_t LogBufferMutexHandle;
extern osMutexId_t FingerprintMutexHandle;
#endif /* app */

extern osEventFlagsId_t GlobalEventHandle;
extern osEventFlagsId_t FreeRTOS_Event_CANHandle;
extern osEventFlagsId_t FreeRTOS_Event_FOTAHandle;
extern osEventFlagsId_t FreeRTOS_Event_MQTTHandle;
#if APP
extern osEventFlagsId_t FreeRTOS_Event_IOHandle;
extern osEventFlagsId_t FreeRTOS_Event_RemoteHandle;
extern osEventFlagsId_t FreeRTOS_Event_FingerptintHandle;
#endif /* app */


void OS_Services_Init(void)
{
  // OS_Trd_Main = MainTaskHandle;
  // OS_Trd_CAN = CanTaskHandle;
  // OS_Trd_Modem = ModemTaskHandle;
  #if APP
  // OS_Trd_MQTT = MqttTaskHandle;
  // OS_Trd_IO = IOTaskHandle;
  // OS_Trd_Command = CommandTaskHandle;
  // OS_Trd_Remote = RemoteTaskHandle;
  // OS_Trd_Fingerprint = FingerprintTaskHandle;
  // OS_Trd_Audio = AudioTaskHandle;
  #endif /* APP */

  // OS_Evt_Global = GlobalEventHandle;
  // OS_Evt_CAN = FreeRTOS_Event_CANHandle;
  #if APP
  // OS_Evt_IO = FreeRTOS_Event_IOHandle;
  OS_Evt_MQTT = FreeRTOS_Event_MQTTHandle;
  // OS_Evt_Remote = FreeRTOS_Event_RemoteHandle;
  // OS_Evt_Fingerprint = FreeRTOS_Event_FingerptintHandle;
  #endif /* APP */
  #if BOOTLOADER
  // OS_Evt_FOTA = FreeRTOS_Event_FOTAHandle;
  #endif /* BOOTLOADER */

  #if APP
  // OS_Queue_VehicleState = BikeStateQueueHandle;
  // OS_Queue_Command = CommandQueueHandle;
  // OS_Queue_IoT_Packet = IoT_PacketQueueHandle;
  #endif /* APP */

  // OS_Mtx_Storage = EepromMutexHandle;
  #if APP
  // OS_Mtx_MQTT = MqttMutexHandle;
  // OS_Mtx_VehicleState = VehicleStateMutexHandle;
  // OS_Mtx_LogBuffer = LogBufferMutexHandle;
  // OS_Mtx_Fingerprint = FingerprintMutexHandle;
  #endif /* APP */
}
