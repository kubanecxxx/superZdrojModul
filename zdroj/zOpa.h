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
void opaSetEnabled(bool enabled);
inline void opaSetDisabled(bool disabled);
bool opaIsThermalFailure(void);
inline bool opaIsEnabled(void);
inline uint16_t opaGetCurrentLimitSetup(void);
inline uint16_t opaGetOutputVoltageSetup(void);

#endif /* ZOPA_H_ */
