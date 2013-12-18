/**
 * @file zdroj.c
 * @author kubanec
 * @date 15.7.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "ch.h"
#include "hal.h"
#include "zdroj.h"
#include "zAD.h"
#include "zOpa.h"
#include "zConverter.h"
#include "zLimit.h"

/**
 * @defgroup zdroj
 * @brief API pro ovládání celyho zdroje, nic jinyho uživatel nebude potřebovat
 * @{
 */

/**
 * @brief nastavení celyho zdroje
 *
 * @details defaultní výstupní hodnoty
 *  + output voltage 150mV
 *  + output disabled
 */
void zdrInit(void)
{
	conInit();
	adInit();
	opaInit();
	zLimInit();

	conSetVoltage(150);
}

/**
 * @brief
 * nastavení celyho zdroje jednim přikazem přes strukturu
 */
void zdrSetup(zdr_t * zdr)
{
	zdrSetEnabled(zdr->en);
	zdrSetVoltage(zdr->mv);
	zdrSetCurrentLimit(zdr->ma);
	zdr->current = zdrGetCurrent();
	zdr->voltage = zdrGetVoltage();
	zdr->voltageCon = zdrGetVoltageConverter();
	zdr->enabled = zdrIsOutputEnabled();
	zdr->thermalFail = zdrIsThermalFailure();
	zdr->currentLimit = zdrIsCurrentLimited();
}

/**
 * @brief nastaví výstupní napětí zdroje
 * @param[in] výstupní napětí v mV
 *
 * zároveň si poladi předměnič
 */
void zdrSetVoltage(uint16_t mV)
{
	uint16_t menic;

	//napětí o pět voltů víc poleze z měniče
	//minimálně aspon 10V
	menic = mV + 5000;
	if (menic < 10000)
		menic = 10000;

	conSetVoltage(menic / 100);

	opaSetVoltage(mV);
}

/**
 * @brief nastaví proudovy omezeni zdroje
 * @param[in] proudové omezení v mA
 */
void zdrSetCurrentLimit(uint16_t mA)
{
	opaSetCurrent(mA);
}

/**
 * @brief funkce vrací hodnotu napětí za měničem
 * @return  napětí v mV
 */
uint16_t zdrGetVoltageConverter(void)
{
	return conGetVoltage();
}

/**
 * @brief funkce vrací hodnotu napětí na výstupu zdroje
 * @return  napětí v mV
 */
uint16_t zdrGetCurrent(void)
{
	return opaGetOutputCurrent();
}

/**
 * @brief funkce vrací hodnotu hodnotu proudu do zátěže
 * @return  proud v mA
 */
uint16_t zdrGetVoltage(void)
{
	return opaGetOutputVoltage();
}

/**
 * @brief funkce připojí výstupní napětí k výstupním svorkám
 */
void zdrSetEnabled(bool_t enable)
{
	opaSetEnabled(enable);
}

/**
 * @brief funkce odpojí výstupní napětí od výstupních svorek
 */
void zdrSetDisabled(bool_t disable)
{
	opaSetDisabled(disable);
}

/**
 * @brief funkce na zjištění přehřátí zdroje
 * @return
 *  + TRUE pokud je přehřáto
 *  + FALSE pokud je v pořádku
 */
bool_t zdrIsThermalFailure(void)
{
	return opaIsThermalFailure();
}

/**
 * @brief funkce na zjištění stavu výstupu zdroje
 * @return
 *  + TRUE pokud je výstup aktivní
 *  + FALSE pokud je výstup vypnutý
 */
bool_t zdrIsOutputEnabled(void)
{
	return opaIsEnabled();
}

/**
 * @brief funkce schroustá data v bufferu od AD převodníku
 */
void zdrProcessData(void)
{
	adProcessData();
}

/**
 * @brief vrátí stav záběru proudové pojistky
 * @return
 *  + TRUE pokud je aktivni proudovy omezeni
 *  + FALSE pokud je všecko OK
 */
bool_t zdrIsCurrentLimited(void)
{
	return zLimIsCurrentLimited();
}

/**
 * @brief ladící funkce na přímé ovládání
 * D/A převodníků
 */
#ifdef ADC_DEBUG
#include "zDA.h"

static volatile uint16_t vVoltage = 0, vCurr = 0;
static volatile uint16_t tv = 0, tc = 0;
static volatile uint16_t s = 1;

void zdrDacRoutine(void)
{
	if (s)
	{
		daInit();
		s = 0;
	}
	if (tv != vVoltage)
	{
		daSetVoltage(1, vVoltage);
		tv = vVoltage;
	}
	if (tc != vCurr)
	{
		daSetVoltage(2, vCurr);
		tc = vCurr;
	}

}
#endif

/**
 * @}
 */
