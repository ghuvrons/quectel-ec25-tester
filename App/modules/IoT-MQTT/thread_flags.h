/*
 * thread_flags.h
 *
 *  Created on: Mar 4, 2022
 *      Author: janoko
 */

#ifndef APP_MODULES_IOT_MQTT_THREAD_FLAGS_H_
#define APP_MODULES_IOT_MQTT_THREAD_FLAGS_H_

#define MQTT_F_START          0x00000001U
#define MQTT_F_STOP           0x00000002U
#define MQTT_F_CONNECT        0x00000004U
#define MQTT_F_DISCONNECT     0x00000008U
#define MQTT_F_SEND_REPORT    0x00000010U
#define MQTT_F_SEND_CMD_ACK   0x00000020U
#define MQTT_F_ALL            MQTT_F_START|MQTT_F_STOP|MQTT_F_CONNECT|MQTT_F_DISCONNECT\
                              |MQTT_F_SEND_REPORT|MQTT_F_SEND_CMD_ACK

#endif /* APP_MODULES_IOT_MQTT_THREAD_FLAGS_H_ */
