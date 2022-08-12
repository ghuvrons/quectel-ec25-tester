/*
 * rng.c
 *
 *  Created on: Mar 8, 2021
 *      Author: Pudja Mansyurin
 */

/* Includes
 * --------------------------------------------*/
#include "random_int.h"
#include <rng.h>
#include <os.h>


/* External variables
 * --------------------------------------------*/
extern osMutexId_t RngMutexHandle;

/* Private variables
 * --------------------------------------------*/
static RNG_HandleTypeDef *prng = &hrng;

/* Private function prototypes
 * --------------------------------------------*/
static void Lock(void);
static void UnLock(void);

/* Public function implementations
 * --------------------------------------------*/
bool RNG_Generate32(uint32_t *data, uint8_t size)
{
  bool ok;

  Lock();
  do
    ok = HAL_RNG_GenerateRandomNumber(prng, data++) == HAL_OK;
  while (ok && --size);
  UnLock();

  return (ok);
}

/* Private function implementations
 * --------------------------------------------*/
static void Lock(void)
{
}

static void UnLock(void)
{
}
