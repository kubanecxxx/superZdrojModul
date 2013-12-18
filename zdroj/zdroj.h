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
INLINE bool_t zdrIsCurrentLimited(void);

/**
 * @brief
 * ladící struktura pro ovládání zdroje jednim přikazem
 */
typedef struct
{
	uint16_t mv;
	uint16_t ma ;
	bool_t en;
	uint16_t current;
	uint16_t voltage;
	uint16_t voltageCon;

	bool_t enabled;
	bool_t thermalFail;
	bool_t currentLimit;
} zdr_t;

void zdrSetup(zdr_t * zdr);

#ifdef ADC_DEBUG
void zdrDacRoutine(void);
#endif

#endif /* ZDROJ_H_ */
