/*
 * rng.h
 *
 *  Created on: Mar 8, 2021
 *      Author: Pudja Mansyurin
 */

#ifndef APP_MODULES_RANDOM_INT_H_
#define APP_MODULES_RANDOM_INT_H_

/* Includes
 * --------------------------------------------*/
#include <_defs/general.h>

/* Public function prototypes
 * --------------------------------------------*/
bool RNG_Generate32(uint32_t *data, uint8_t size);

#endif /* APP_MODULES_RANDOM_INT_H_ */
