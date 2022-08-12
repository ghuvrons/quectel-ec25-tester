/*
 * types.h
 *
 *  Created on: Mar 4, 2022
 *      Author: janoko
 */

#ifndef DEFS_TYPES_H_
#define DEFS_TYPES_H_


/**
 * Date and time
 */
typedef struct __attribute__((packed))
{
  uint8_t year;
  uint8_t month;
  uint8_t date;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} Datetime_t;

typedef uint32_t Timestamp_t;

#define Datetime_GetWeekday(dt) (((dt)->date + (2.6*(dt)->month - 0.2) - 2*20.0 + (dt)->year + 20/4 + (dt)->year/4) % 7)

/**
 * Union for CAN Bus
 */
typedef union
{
  uint8_t u8[8];
  uint16_t u16[4];
  uint32_t u32[2];
  uint64_t u64;
  int8_t i8[8];
  int16_t i16[4];
  int32_t i32[2];
  int64_t i64;
  char CHAR[8];
  float FLOAT[2];
  double DOUBLE;
} Union64_t;

/**
 * User Setting
 */
typedef struct {
  uint8_t driveMode;
  uint8_t maxSpeed;
} UserSetting_t;


/**
 * Trip types
 */
#define TRIP_000    0
#define TRIP_A      1
#define TRIP_B      2
#define TRIP_MAX    3
#define TRIP_MAX_M  99999999.00

typedef uint8_t TripMode_t;
typedef float TripMeter_t;

typedef void (*VoidCallback)(void);
typedef void (*BufferCallback)(const char *data, uint16_t size);
typedef void (*MqttInboxCallback)(uint16_t id, uint8_t qos,
                                  const char *topic, uint16_t topicSz,
                                  const char *data, uint16_t dataSz);

#endif /* DEFS_TYPES_H_ */
