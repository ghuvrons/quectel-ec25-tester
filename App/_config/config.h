/*
 * config.h
 *
 *  Created on: Dec 15, 2021
 *      Author: janoko
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_
/* */
#define ENV_FIRMWARE_V 2

/* os */
#define USE_FREERTOS 1

/* Enabling feature */
#define EN_FEATURE_FGR            0
#define EN_FEATURE_KEYLESS        0
#define EN_FEATURE_HB_SELECT_SET  1
#define EN_FEATURE_IWDG           1

/* DEBUG */
#define DEBUG_CAN   1
#define DEBUG_STG   1
#define DEBUG_DATA  0
#define DEBUG_SIM   1
#define DEBUG_FOTA  0
#define DEBUG_MQTT  1
#define DEBUG_RMT   0
#define DEBUG_IMU   0


/* Network */

#define CONF_NTP_SERVER "time.google.com"

#define CONF_APN      "iot.truphone.com"
#define CONF_APN_USER ""
#define CONF_APN_PASS ""

#define CONF_MQTT_SERVER_HOST "mqtt.farad-ev.com"
#define CONF_MQTT_SERVER_PORT 1883
#define CONF_MQTT_USERNAME    "farad-ev"
#define CONF_MQTT_PASSWORD    "Vr@467890"


#endif
