/**
 * @file zLimit.c
 * @author kubanec
 * @date 28.8.2013
 *
 */

#define _ZDROJ_SUBMODULES
#include "zlock.h"

#include "ch.h"
#include "hal.h"
#include "scheduler.h"
#include "zOpa.h"
#include "zConverter.h"

/**
 * @ingroup zdroj
 * @defgroup Limit
 * @brief kontrolní modul pro vyhodnocení překročení
 *  proudu + snížení napětí předměniče at se zbytečně nepálí opa
 *
 *  @details
 *  periodicky volaná funkce vyhodnocuje ješli opa už omezuje proud
 *  vyhodnocení proběhne tak že výstupní napětí je o x% menšní než nastaveny
 *  a zároveň výstupní proud bude už větši než x% z nastavenyho limitu
 *
 * @{
 */

static void zLimThread(arg_t arg);

/**
 * @brief interní proměnná pro zaznamenání že je proudová pojistka aktivní
 */
static bool_t zLimLimitation = FALSE;

/**
 * @brief inicializuje modul, jenom vytvoří periodickou rutinu pro scheduler
 * \ref zLimThread
 */
void zLimInit(void)
{
	static delay_t del;
	shFillStruct(&del, zLimThread, NULL, MS2ST(50), PERIODIC);
	shRegisterStruct(&del);

#ifdef DEBUG_LED
	palSetPadMode(GPIOC,15,PAL_MODE_OUTPUT_PUSHPULL);
#endif
}

/**
 * @brief rutina je volaná každéch 100ms
 * @details
 * vyhodnocuje jesli došlo k proudovymu omezeni
 * stylem že výstupní proud už je přes 90% omezení a výstupní napětí
 * je menší než 90% nastavenyho
 */
void zLimThread(arg_t arg)
{
	(void) arg;

	//potřebuju vystupní proud, limit, vystupní napětí, nastaveny vystupni napěti
	static uint16_t volReal = 0;
	static uint16_t volUser = 0;
	static uint16_t curReal = 0;
	static uint16_t curUser = 0;

	volReal = opaGetOutputVoltage();
	volUser = opaGetOutputVoltageSetup();
	curReal = opaGetOutputCurrent();
	curUser = opaGetCurrentLimitSetup();

	//kruciální podmínka
	uint32_t curTemp, volTemp;
	curTemp = curUser * 9;
	curTemp /= 10; //90%

	volTemp = volUser * 9;
	volTemp /= 10; //90%

	zLimLimitation = FALSE;
	if (curTemp < curReal)
		if (volTemp > volReal)
			zLimLimitation = TRUE;

	// todo snížit napětí z měniče pokud bude zabirat proudová pojistka

#ifdef DEBUG_LED
	if (zLimLimitation)
		palSetPad(GPIOC,15);
	else
		palClearPad(GPIOC,15);
#endif
}

/**
 * @brief vrací jestli je zdroj v oblasti limitace proudu
 * @return
 *  - TRUE pokud už proudová pojistka zabírá
 *  - FALSE pokud je proudová pojistka neaktivní
 */
bool_t zLimIsCurrentLimited(void)
{
	return zLimLimitation;
}

/**
 * @}
 */
