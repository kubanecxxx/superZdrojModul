/**
 * @file zConverter.h
 * @author kubanec
 * @date 14.7.2013
 *
 */

#ifndef ZCONVERTER_H_
#define ZCONVERTER_H_

#include "zlock.h"

bool conSetVoltage(uint16_t voltage);
uint16_t conGetVoltage(void);
void conInit(void);

#endif /* ZCONVERTER_H_ */
