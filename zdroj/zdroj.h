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
inline void zdrSetCurrentLimit(uint16_t mA);
inline uint16_t zdrGetVoltage(void);
inline uint16_t zdrGetCurrent(void);
inline uint16_t zdrGetVoltageConverter(void);
inline void zdrSetEnabled(bool enabled);
inline void zdrSetDisabled(bool disabled);
inline bool zdrIsOutputEnabled(void);
inline bool zdrIsThermalFailure(void);
inline void zdrProcessData(void);
inline bool zdrIsCurrentLimited(void);
inline uint16_t zdrGetVoltageConverterLatched(void);

/**
 * @brief
 * ladící struktura pro ovládání zdroje jednim přikazem
 */
typedef struct
{
	uint16_t mv;
	uint16_t ma ;
    bool en;
	uint16_t current;
	uint16_t voltage;
	uint16_t voltageCon;

    bool enabled;
    bool thermalFail;
    bool currentLimit;
} zdr_t;

void zdrSetup(zdr_t * zdr);

#ifdef ADC_DEBUG
void zdrDacRoutine(void);
#endif

#endif /* ZDROJ_H_ */
