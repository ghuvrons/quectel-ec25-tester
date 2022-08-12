/*
#include <ioT/iot.h>
 * iot.c
 *
 *  Created on: Dec 20, 2021
 *      Author: janoko
 */

#include "mqtt.h"
#include "commands.h"
#include "thread_flags.h"
#include <modem.h>
#include <dma_streamer.h>
#include <os.h>
#include <utils/random/random_int.h>
#include <buffer.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <quectel/http.h>

#define MQTT_F_RECV_START  0x00000001U
#define MQTT_F_RECV_STOP   0x00000002U

#define MAX_LOOP_CTR 10

#if DEBUG_MQTT
#include <stdarg.h>
#include <utils/debugger.h>
#define MQTT_Println(...) {DBG_Printf("MQTT: ");DBG_Println(__VA_ARGS__);}
#else
#define MQTT_Println(...) {}
#endif

#define TOPIC_TMP_STATUS    "VCU/%u/STS"
#define TOPIC_TMP_REPORT    "VCU/%u/RPT"
#define TOPIC_TMP_COMMAND   "VCU/%u/CMD"
#define TOPIC_TMP_RESPONSE  "VCU/%u/RSP"


MQTT_HandlerTypeDef Mod_MQTT;

static void onSocketConnecting(void);
static void onSocketConnected(void);
static void onSocketReceived(Buffer_t*);
static void onSocketClosed(void);
static void onIncomingCMD(MessageData* data);

static void networkInit(MqttNetwork*);
static int socketSendData(MqttNetwork*, unsigned char *data, int datalength, int timeout);
static int socketRecvData(MqttNetwork*, unsigned char *buffer, int len, int timeout);

extern osMutexId_t MqttMutexHandle;

static uint8_t      socketBuffer[MQTT_SOCKET_BUUFER_LEN];
static uint8_t      txBuffer[MQTT_TXPACKET_BUF_LEN]; // packet buffer
static uint8_t      rxBuffer[MQTT_RXPACKET_BUF_LEN]; // packet buffer
static MqttNetwork  mqttNetwork;

// strings buffers
static char clientId[25];
static char mqttServerHost[25];
static char mqttUsername[25];
static char mqttPassword[25];
static char topicStatus[32];
static char topicReport[32];
static char topicCommand[32];
static char topicResponse[32];

void MQTT_Init(void)
{
  uint32_t random;

  sprintf(mqttServerHost, CONF_MQTT_SERVER_HOST);
  sprintf(mqttUsername,   CONF_MQTT_USERNAME);
  sprintf(mqttPassword,   CONF_MQTT_PASSWORD);

  Mod_MQTT.server.host      = mqttServerHost;
  Mod_MQTT.server.port      = CONF_MQTT_SERVER_PORT;
  Mod_MQTT.server.username  = mqttUsername;
  Mod_MQTT.server.password  = mqttPassword;

  sprintf(topicStatus,    TOPIC_TMP_STATUS,   72);
  sprintf(topicReport,    TOPIC_TMP_REPORT,   72);
  sprintf(topicCommand,   TOPIC_TMP_COMMAND,  72);
  sprintf(topicResponse,  TOPIC_TMP_RESPONSE, 72);

  RNG_Generate32(&random, 1);
  sprintf(clientId, "VCU-%d-%d", 72, (int) (uint8_t) random);

  networkInit(&mqttNetwork);
}


void MQTT_Run(void *args)
{
  uint32_t notif;
  MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
  MQTTMessage publishData;
  MqttTimer incomingPacketTimer;
  int loop_ctr = 0;
  int rc = 0;

  // OS_WaitReady();
  TimerInit(&incomingPacketTimer);

  start:
  MQTT_Println("Waiting modem ready");
  if (!MDM_WaitReady(6000)) {
    goto start;
  }
  MQTT_Init();

  loop_ctr = 0;
  reconnect:
  if (loop_ctr++ > MAX_LOOP_CTR) {
    MDM_Reset();
    goto start;
  }

  MQTT_Println("Waiting socket connected. socket state : (%d)", Mod_MQTT.socket.state);
  if (QTEL_SOCK_IS_STATE(&Mod_MQTT.socket, QTEL_SOCK_STATE_OPEN))
    QTEL_SOCK_Close(&Mod_MQTT.socket);
  if (OS_IsError(OS_EventWait(OS_Evt_MQTT, MQTT_F_CONNECT, &notif, OS_WaitAny, 30000))) {
    goto reconnect;
  }

  OS_EventClear(OS_Evt_MQTT, MQTT_F_ALL);

  MQTTClientInit(&(Mod_MQTT.mqttClient), &mqttNetwork, 30000,
                 txBuffer, MQTT_TXPACKET_BUF_LEN,
                 rxBuffer, MQTT_RXPACKET_BUF_LEN);

  connectData.MQTTVersion = 3;
  connectData.keepAliveInterval = 30;
  connectData.clientID.cstring = clientId;
  connectData.username.cstring = Mod_MQTT.server.username;
  connectData.password.cstring = Mod_MQTT.server.password;

  connectData.willFlag = 1;
  connectData.will.qos = 1;
  connectData.will.retained = 1;
  connectData.will.topicName.cstring = topicStatus;
  connectData.will.message.cstring = "0";

  MQTT_Println("connecting to (%s:%d) [id:%s]...",
               Mod_MQTT.server.host, Mod_MQTT.server.port, clientId);

  if ((rc = MQTTConnect(&(Mod_MQTT.mqttClient), &connectData)) != 0) {
    MQTT_Println("Return code from MQTT connect is %d\n", rc);
    goto disconnect;
  }
  MQTT_OnConnected();

  MQTT_Println("connected");

  // publish status
  publishData.qos = 1;
  publishData.retained = 1;
  publishData.payload = "1";
  publishData.payloadlen = 1;

  if ((rc = MQTTPublish(&(Mod_MQTT.mqttClient), topicStatus, &publishData)) != STATUS_SUCCESS) {
    MQTT_Println("publishing error [ecode: %d]", rc);
    goto disconnect;
  }

  if ((rc = MQTTSubscribe(&(Mod_MQTT.mqttClient), topicCommand, 2, onIncomingCMD)) != STATUS_SUCCESS) {
    MQTT_Println("subscribing error [ecode: %d]", rc);
    goto disconnect;
  }
  MQTT_Println("subscribed topic: %s", topicCommand);
  /* Infinite loop */
  for(;;)
  {
    if (!Mod_MQTT.mqttClient.isconnected) goto disconnect;

    if (!OS_IsError(OS_EventWait(OS_Evt_MQTT, MQTT_F_SEND_REPORT|MQTT_F_DISCONNECT|MQTT_F_SEND_CMD_ACK, &notif, OS_WaitAny, 100))) {
      if (notif & MQTT_F_DISCONNECT)    goto disconnect;
      if (notif & MQTT_F_SEND_CMD_ACK)  MQTT_CommandAck();
      continue;
    }
    MQTTCheckIncomingPacket(&(Mod_MQTT.mqttClient), &incomingPacketTimer);
    MQTT_Report();
  }

  disconnect:
  loop_ctr = 0;
  goto reconnect;
}

__weak void MQTT_OnConnected(void) {}
__weak void MQTT_Report(void) {}


bool MQTT_SendReport(void *payload, uint16_t length)
{
  int rc = 0;
  MQTTMessage publishData;

  if (!Mod_MQTT.mqttClient.isconnected) return false;

  publishData.qos = 1;
  publishData.retained = 1;
  publishData.payload = payload;
  publishData.payloadlen = length;
  if ((rc = MQTTPublish(&(Mod_MQTT.mqttClient), topicReport, &publishData)) != STATUS_SUCCESS) {
    MQTT_Println("report publishing error [ecode: %d]", rc);
    return false;
  }
  return true;
}


__weak void MQTT_CommandHandler(void *payload, uint16_t length) {}
__weak void MQTT_CommandAck(void) {}


bool MQTT_SendResponse(void *payload, uint16_t length)
{
  MQTTMessage publishData;

  if (!Mod_MQTT.mqttClient.isconnected) return false;

  publishData.qos = 1;
  publishData.payload = payload;
  publishData.payloadlen = length;
  if (MQTTPublish(&(Mod_MQTT.mqttClient), topicResponse, &publishData) != STATUS_SUCCESS) {
    MQTT_Println("response publishing error");
    return false;
  }
  return true;
}

void TimerInit(MqttTimer *timer)
{
  timer->lastTick = 0;
  timer->timeout = 0;
}

char TimerIsExpired(MqttTimer *timer)
{
  // is timeout
  if (OS_GetTick() - timer->lastTick > timer->timeout) return 1;
  return 0;
}

void TimerCountdownMS(MqttTimer *timer, unsigned int cd)
{
  timer->lastTick = OS_GetTick();
  timer->timeout = cd;
}

void TimerCountdown(MqttTimer *timer, unsigned int cd)
{
  TimerCountdownMS(timer, cd*1000);
}

int TimerLeftMS(MqttTimer *timer)
{
  uint32_t currentTick = OS_GetTick();
  return (timer->lastTick+timer->timeout < currentTick)? 0: (int) timer->lastTick+timer->timeout - currentTick;
}


void MutexInit(MqttMutex* mutex)
{
  mutex->mtx = 0;
}

int MutexLock(MqttMutex* mutex)
{
  return OS_MutexAcquire(mutex->mtx, osWaitForever);
}

int MutexUnlock(MqttMutex* mutex)
{
  return OS_MutexRelease(mutex->mtx);
}


static void onSocketConnecting(void)
{
  MQTT_Println("socket connecting");
  Mod_MQTT.socketState = MQTT_SOCKET_STATE_CCONNECTING;
}


static void onSocketConnected(void)
{
  MQTT_Println("socket connected");
  Mod_MQTT.socketState = MQTT_SOCKET_STATE_AVAILABLE;
  mqttNetwork.isConnected = 1;
  OS_EventSet(OS_Evt_MQTT, MQTT_F_CONNECT);
}


static void onSocketReceived(Buffer_t *buf)
{
}

static void onSocketClosed(void)
{
  MQTT_Println("socket closed");
  Mod_MQTT.socketState = MQTT_SOCKET_STATE_CLOSED;
  mqttNetwork.isConnected = 0;
  OS_EventSet(OS_Evt_MQTT, MQTT_F_DISCONNECT);
}

static void networkInit(MqttNetwork *network)
{
  // init socket
  Mod_MQTT.socket.config.autoReconnect = 1;
  Mod_MQTT.socket.config.reconnectingDelay = 1000;
  Mod_MQTT.socket.listeners.onConnecting = onSocketConnecting;
  Mod_MQTT.socket.listeners.onConnected = onSocketConnected;
  Mod_MQTT.socket.listeners.onReceived = onSocketReceived;
  Mod_MQTT.socket.listeners.onClosed = onSocketClosed;

  // init mqtt network
  network->socket = (void*) &Mod_MQTT.socket;
  network->isConnected = 0;
  network->mqttwrite = socketSendData;
  network->mqttread = socketRecvData;

  QTEL_SOCK_SetBuffer(&(Mod_MQTT.socket), &(socketBuffer[0]), MQTT_SOCKET_BUUFER_LEN);
  QTEL_SOCK_Init(&(Mod_MQTT.socket), Mod_MQTT.server.host, Mod_MQTT.server.port);
  QTEL_SOCK_Open(&(Mod_MQTT.socket), &(Mod_Modem.driver));
}

static int socketSendData(MqttNetwork *socket, unsigned char *buffer, int len, int timeout)
{
  uint16_t sendLen = QTEL_SOCK_SendData((QTEL_Socket_t*) socket->socket, buffer, len);

  #if DEBUG_MQTT&0
  DBG_Println("========= Sending Data ==========");
  DBG_PrintB(buffer, (uint16_t) len);
  DBG_Println("Sent (%dB)", sendLen);
  DBG_Println("=================================");
  #endif

  if (sendLen != 0) return sendLen;
  return -1;
}

static int socketRecvData(MqttNetwork *network, unsigned char *buffer, int len, int timeout)
{
  QTEL_Socket_t *socket = (QTEL_Socket_t *) network->socket;
  Buffer_t *socketBuffer = &socket->buffer; //&(Mod_MQTT.socket.buffer);
  int recvLen = 0;
  int32_t tmpReadLen = 0;
  uint32_t lastTick = OS_GetTick();

  do {
    if (QTEL_SOCK_IS_STATE(socket, QTEL_SOCK_STATE_CLOSED)) {
      network->isConnected = 0;
      return 0;
    }
    if (Buffer_IsAvailable(socketBuffer)) {
      tmpReadLen = Buffer_Read(socketBuffer, buffer + recvLen, len - recvLen);

      if (tmpReadLen > 0) {
        recvLen += (int) tmpReadLen;
      }
    }
    OS_Delay(1);
  } while (recvLen < len && OS_GetTick() - lastTick < timeout);

  return recvLen;
}

static void onIncomingCMD(MessageData* data)
{
  // return if not valid
  if (data->message->payloadlen < sizeof(MQTT_Header_t)) return;

  MQTT_Println("Incoming command");
  MQTT_CommandHandler(data->message->payload, data->message->payloadlen);
  OS_EventSet(OS_Evt_MQTT, MQTT_F_SEND_CMD_ACK);
}
