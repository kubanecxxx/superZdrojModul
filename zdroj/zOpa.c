/**
 * @file zOpa.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "ch.h"
#include "hal.h"
#include "zOpa.h"
#include "zDA.h"

/**
 * @ingroup zdroj
 * @defgroup opa
 * @brief API pro ovládání OPA548 interním DA převodníkem
 * @{
 */

/**
 * @brief paměť že je výstup zdroj zapnuté
 */
static bool_t enabled = FALSE;
/**
 * @brief latch výstupního napětí
 */
static uint16_t outputVoltage = 0;
/**
 * @brief latch výstupního proudu
 */
static uint16_t currentLimit = 0;

/**
 *	@brief
 * proměnné pro nastavení od AD převodníku; uživatel nesmí používat;
 * nastavováno v \ref AD
 */
uint16_t _adNapeti;
/**
 *	@brief
 * proměnné pro nastavení od AD převodníku; uživatel nesmí používat;
 * nastavováno v \ref AD
 */
uint16_t _adProud1;

/**
 * @brief enum pro lepši rozpoznání kanálů DA převodniku
 */
typedef enum
{
	/// proud
	CURRENT = 2,
	/// napětí
	VOLTAGE = 1
} DAChannel_t;

/// port ES - čtení thermal failure a ovládání výstupu
#define ES_GPIO GPIOA
/// pin ES
#define ES_PIN 7

/**
 * @brief inicializace zdroje OPA + init DA převodníků
 */
void opaInit(void)
{
	daInit();
	opaSetDisabled(TRUE);
}

/**
 * @brief nastaví výstupní napětí OPA548
 * @param [in] mV napětí v mV
 *
 * musí přepočítat výstupní napětí přes zesílení OPA na napětí kterym ho má krmit
 */
void opaSetVoltage(uint16_t mV)
{
	uint16_t temp;
	outputVoltage = mV;

	temp = mV / (zConstants.opaVoltR2 / zConstants.opaVoltR1 + 1); //mV / 11
	daSetVoltage(VOLTAGE, temp);
}

/**
 * @brief nastaví proudovou pojistku OPA548
 * @param [in] mA maximální proud v mA
 *
 * musí přepočítat pojistku na napěti kterym ho má krmit
 * na výstupu DA je ještě zesilovač 2x
 */
void opaSetCurrent(uint16_t mA)
{
	//Vset = 4.75 - (13750*Ilim / 15000)
	//1000Vset = 4750 - (13750 * Ilim / 15)

	currentLimit = mA;

	uint32_t temp = 13750L * mA;
	uint32_t tme;
	uint32_t korigovano;
	temp /= 15000;
	temp = 4880 - temp;
	//@todo 4750 hodit do konstant protože u druhyho bude asi jinak
	//děleno dvěma protože na vystupu je zesilovač 2x
	tme = temp / 2;
	//tme += 50;
	//@todo dalši konstanta
	//zesilovač nemá přesně zesileni 2x
	korigovano = tme * 10312;
	korigovano /= 10000;

	daSetVoltage(CURRENT, tme);
}

/**
 * @brief zapne nebo vypne výstup OPA podle parametru
 * @param [in] enable
 * 	+ TRUE výstup se zapne
 * 	+ FALSE výstup se vypne
 */
void opaSetEnabled(bool_t enable)
{
	enabled = enable;

	if (enable == FALSE)
	{
		//vypnout
		palSetPadMode(ES_GPIO,ES_PIN, PAL_MODE_OUTPUT_OPENDRAIN);
		palClearPad(ES_GPIO,ES_PIN);
	}
	else
	{
		//zapnout - jenom přepne jako vstup aby mohl snimat přehřátí
		palSetPadMode(ES_GPIO,ES_PIN,PAL_MODE_INPUT);
	}
}

/**
 * @brief zapne nebo vypne výstup OPA podle parametru
 * @param [in] disable
 * 	+ TRUE výstup se vypne
 * 	+ FALSE výstup se zapne
 */
void opaSetDisabled(bool_t disable)
{
	opaSetEnabled(!disable);
}

/**
 * @brief vrátí jesli je OPA přehřáty
 * @return
 * + TRUE pokud je přehřáté
 * + FALSE pokue je v pohodě
 *
 * testuje jesli je výstup aktivní, přehřátí je pak značeno přizeměním ES
 */
bool_t opaIsThermalFailure(void)
{
	if (opaIsEnabled())
	{
		if (palReadPad(ES_GPIO,ES_PIN) == 0)
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief zavrátí jesli je výstup zdroje zapnuté nebo ne
 */
bool_t opaIsEnabled(void)
{
	return enabled;
}

/**
 * @brief změří výstupní napětí za OPA
 * @return výstupní napětí v mV
 */
uint16_t opaGetOutputVoltage(void)
{
	return ODDELIC(zConstants.measOutVoltR1,zConstants.measOutVoltR2,_adNapeti);
}

/**
 * @brief změří výstupní proud do zátěže
 * @return aktuální výstupní proud v mA
 */
uint16_t opaGetOutputCurrent(void)
{
	/*
	 * bude sranda dopočitat
	 * bude lepši ladit až přimo na zátěži
	 */
	//20V per 10A
	uint16_t temp = _adProud1 * zConstants.measCurMul;
	temp = temp / zConstants.measCurDiv;
	return temp;
}

/**
 * @brief vrátí hodnotu proudovyho limitu která byla předtim nastavena v mA
 */
uint16_t opaGetCurrentLimitSetup(void)
{
	return currentLimit;
}

/**
 * @brief vrátí nastavenou hodnotu napětí v mV
 */
uint16_t opaGetOutputVoltageSetup(void)
{
	return outputVoltage;
}

/**
 * @}
 */
