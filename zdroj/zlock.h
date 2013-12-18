/**
 * @file lock.h
 * @author kubanec
 * @date 17.7.2013
 *
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifndef _ZDROJ_SUBMODULES
#error "zfiles can be included only in zdroj.c"
#endif

#include <stdint.h>

/**
 * @ingroup zlock
 * @{
 */

/**
 * @brief sada konstant pro trimrování zdroje
 */
typedef struct zConstants_t zConstants_t;

/**
 * @brief sada konstant pro trimrování zdroje
 *
 * postup pro zjištění konstant:
 * nejprve zkalibrovat D/A převodníky - napojit na výstupy měřáky a
 * štelovat konstantu dokud nebude hodnota na měřáku stejná jako v allData
 * daVoltageMv
 * nebo použít DA kalibrační firmware přepnutím v chconf
 * hodnota vychází kolem 1200
 *
 * opaVoltR1,R2 až bude D/A převodnik tak si nastavit nějaky napěti vystupu
 * hodnoty R1,R2 dat implicitně 1K a 10K a pak jednu hodnotu doladit aby bylo
 * napěti na vystupu stejny jako chtěná hodnota v alldata opaVoltageMv
 *
 * opaCurrOZ - D/A převodníku leze napětí do zesilovače 2x a ten nemá
 * zesílení doopravdy 2 - měřit přímo na výstupu OZ (vstupu OPA548) aby tam bylo
 * to samo jak v alldata opaCurrentOZ
 *
 * opaCurrVoltage - ideálně odpojit vstup OPA548 a změřit na něm napětí,
 * bude ho tam mět pokud neni zatížen
 * při tomto napětí je proudová pojistka nastavená na hodnotu 0mA
 * katalogová hodnota je 4750
 *
 * u měření štelovat hodnoty odporu aby to měřilo to samo co měřák
 */
struct zConstants_t
{
	uint16_t measCurMul;///< měření proudu násobení
	uint16_t measCurDiv;///< měření proudu dělení
	uint16_t measOutVoltR1;///< odpor R1 při měření výstupního napětí
	uint16_t measOutVoltR2;///< odpor R2 pro měření výstupního napětí
	uint16_t measConvVoltR1;///< odpor R1 děliče  měření napětí za měničem
	uint16_t measConvVoltR2;///< odpor R2 děliče měření napětí za měničem

	uint16_t opaVoltR1;///< vstupní odpor OPA
	uint16_t opaVoltR2;///< feedback odpor OPA
	/**
	 *  za DA převodnikem je zesilovač 2x
	 *  ale nemá přesně zesílení 2
	 *  timhle čislem se násobijou mV, ktery se maji posilat
	 *  na DA převondik, pak se děli 10000
	 */
	uint16_t opaCurrOZ;
	/**
	 * hodnota zdroje napětí, kterej má v sobě OPA
	 * při tomhle napětí je proud omezenej na minimum
	 * katalogová hodnota je 4750mV
	 */
	uint16_t opaCurrVoltage;

	/**
	 * timhle čislem se násobijou mV co maji jit na D/A převondnik
	 * pak se vydělijou 1000 a pošlou se tam
	 */
	uint16_t refDA1;
	uint16_t refDA2; ///< referenční napětí D/A převodníku MCU v mV

	/**
	 * timhle se násobijou všechny vzorky a pak se dělijou referenčním napetim
	 * hodnota kolem 1200
	 */
	uint16_t refAD;
};

/*
 * @brief
 * sem se ukládaji všecky data ktery sou potřeba
 * pro ovládání zdroje
 */
typedef struct zAllData_t zAllData_t;
struct zAllData_t
{
	/// napětí co naměřim na vystupu DA převodniku měřákem
	uint16_t daVoltageMV;
	uint16_t daCurrentMV;
	uint16_t daConverterMV;

	//hodnoty co se cpou přimo do DA převodniku
	uint16_t daVoltageData;
	uint16_t daCurrentData;

	//hodnoty ktery bych si představoval na vystupech
	uint16_t ConverterVoltageMV;
	uint16_t opaVoltageMV;
	//napětí co má být za OZ
	uint16_t opaCurrentOZ;
};

extern  zConstants_t zConstants;
extern zAllData_t allData;

/**
 * @}
 */

#endif /* LOCK_H_ */
