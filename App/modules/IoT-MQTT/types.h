/*
 * types.h
 *
 *  Created on: Apr 4, 2022
 *      Author: janoko
 */

#ifndef APP_MODULES_MQTT_TYPES_H_
#define APP_MODULES_MQTT_TYPES_H_

#include <os.h>

typedef struct MqttNetwork MqttNetwork;

struct MqttNetwork
{
  void *socket;
  char isConnected;
  int (*mqttread) (MqttNetwork*, unsigned char*, int, int);
  int (*mqttwrite) (MqttNetwork*, unsigned char*, int, int);
  void (*disconnect) (MqttNetwork*);
};

typedef struct {
  uint32_t lastTick;
  uint32_t timeout;
} MqttTimer;

typedef struct MqttMutex
{
  OS_Mutex_t mtx;
} MqttMutex;

#endif /* APP_MODULES_MQTT_TYPES_H_ */
