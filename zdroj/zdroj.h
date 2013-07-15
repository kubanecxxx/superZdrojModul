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
void zdrSetCurrentLimit(uint16_t mA);
uint16_t zdrGetVoltage(void);
uint16_t zdrGetCurrent(void);
inline uint16_t zdrGetVoltageConverter(void);
void zdrSetEnabled(bool_t enabled);
void zdrSetDisabled(bool_t disabled);
bool_t zdrIsThermalFailure(void);

#endif /* ZDROJ_H_ */
