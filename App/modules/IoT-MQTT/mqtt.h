/*
 * iot.h
 *
 *  Created on: Dec 20, 2021
 *      Author: janoko
 */

#ifndef APP_MODULES_MQTT_H_
#define APP_MODULES_MQTT_H_

#include <_defs/general.h>
#include <quectel/socket.h>
#include "types.h"
#include "thread_flags.h"
#include "mqttClient.h"

#define MQTT_RXBUF_LEN          128
#define MQTT_SOCKET_BUUFER_LEN  516
#define MQTT_TXPACKET_BUF_LEN   256
#define MQTT_RXPACKET_BUF_LEN   256

#define MQTT_SOCKET_STATE_CLOSED          0x00
#define MQTT_SOCKET_STATE_CCONNECTING     0x01
#define MQTT_SOCKET_STATE_AVAILABLE       0x02
#define MQTT_SOCKET_STATE_READING_PACKET  0x03


typedef struct {
  uint8_t       status;
  QTEL_Socket_t socket;
  uint8_t       socketState;
  MQTTClient    mqttClient;

  struct {
    char      *host;
    uint16_t  port;
    char      *username;
    char      *password;
  } server;
} MQTT_HandlerTypeDef;

typedef struct __attribute__((packed))
{
  char      prefix[2];
  uint8_t   size;
  uint16_t  version;
  uint32_t  vin;
} MQTT_Header_t;


extern MQTT_HandlerTypeDef Mod_MQTT;

void MQTT_Init(void);
void MQTT_Run(void *args);
void MQTT_OnConnected(void);
void MQTT_Report(void);
bool MQTT_SendReport(void *payload, uint16_t length);
void MQTT_CommandHandler(void *payload, uint16_t length);
void MQTT_CommandAck(void);
bool MQTT_SendResponse(void *payload, uint16_t length);

void TimerInit(MqttTimer*);
char TimerIsExpired(MqttTimer*);
void TimerCountdownMS(MqttTimer*, unsigned int);
void TimerCountdown(MqttTimer*, unsigned int);
int TimerLeftMS(MqttTimer*);

void MutexInit(MqttMutex*);
int MutexLock(MqttMutex*);
int MutexUnlock(MqttMutex*);

#endif /* APP_MODULES_MQTT_H_ */
