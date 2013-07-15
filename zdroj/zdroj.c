/**
 * @file zdroj.c
 * @author kubanec
 * @date 15.7.2013
 *
 */

#include "ch.h"
#include "hal.h"
#include "zdroj.h"
#include "zDA.h"
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
	daInit();
	adInit();


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

/*
 * @}
 */
