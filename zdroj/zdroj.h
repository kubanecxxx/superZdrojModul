/**
 * @file zdroj.h
 * @author kubanec
 * @date 15.7.2013
 *
 */

#ifndef ZDROJ_H_
#define ZDROJ_H_

void zdrInit(void);
void zdrSetVoltage(uint16_t mV);
INLINE void zdrSetCurrentLimit(uint16_t mA);
INLINE uint16_t zdrGetVoltage(void);
INLINE uint16_t zdrGetCurrent(void);
INLINE uint16_t zdrGetVoltageConverter(void);
INLINE void zdrSetEnabled(bool_t enabled);
INLINE void zdrSetDisabled(bool_t disabled);
INLINE bool_t zdrIsOutputEnabled(void);
INLINE bool_t zdrIsThermalFailure(void);
INLINE void zdrProcessData(void);

#endif /* ZDROJ_H_ */
