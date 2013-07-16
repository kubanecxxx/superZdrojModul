/**
 * @file zdroj.c
 * @author kubanec
 * @date 15.7.2013
 *
 */

#include "ch.h"
#include "hal.h"
#include "zdroj.h"
#include "zAD.h"
#include "zOpa.h"
#include "zConverter.h"

/**
 * @defgroup zdroj
 * @brief API pro ovládání celyho zdroje, nic jinyho uživatel nebude potřebovat
 * @{
 */

/**
 * @brief nastavení celyho zdroje
 */
void zdrInit(void)
{
	conInit();
	adInit();
	opaInit();

	conSetVoltage(150);
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

/*
 * @}
 */
