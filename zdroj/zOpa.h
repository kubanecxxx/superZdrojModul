/**
 * @file zOpa.h
 * @author kubanec
 * @date 14.7.2013
 *
 */

#ifndef ZOPA_H_
#define ZOPA_H_

#include "zlock.h"

void opaInit(void);
void opaSetVoltage(uint16_t mV);
void opaSetCurrent(uint16_t mA);
uint16_t opaGetOutputVoltage(void);
uint16_t opaGetOutputCurrent(void);
void opaSetEnabled(bool_t enabled);
INLINE void opaSetDisabled(bool_t disabled);
bool_t opaIsThermalFailure(void);
INLINE bool_t opaIsEnabled(void);
INLINE uint16_t opaGetCurrentLimitSetup(void);
INLINE uint16_t opaGetOutputVoltageSetup(void);

#endif /* ZOPA_H_ */
