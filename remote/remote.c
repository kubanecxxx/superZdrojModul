/**
 * @file remote.c
 * @author kubanec
 * @date 29.8.2013
 *
 */

#include "ch.h"
#include "hal.h"
#include "remote.h"
#include "stm32f10x.h"
#include "scheduler.h"
#include "zdroj.h"
#include "remoteProtocol.h"

/**
 * @ingroup remote
 * @defgroup remote_slave
 * @title remote pro slave
 * @brief slave SPI remote ovládání
 * @{
 */

/**
 * @brief interní pointer na funkci které se použivá
 * při zápisu hodnot do zdroje
 */
typedef void (*fce)(uint16_t value);

/**
 * @brief interní struktura pro zapisování změn nastavení zdroje
 * od SPI
 */
typedef struct
{
	bool_t changed; ///< hodnota byla změněna
	uint16_t value; ///< hodnota do metody
	fce method; ///< metoda co semá volat
} dataWrite_t;

/**
 * @brief interní struktura pro předávání všech hodnot z přerušení od SPI
 * do zdroje a vice-versa
 */
typedef struct
{
	uint16_t returnVals[rCOUNT]; ///< seznam hodnot pro čtení
	dataWrite_t writeVals[wCOUNT]; ///< seznam metod + hodnot pro zápis
} data_t;

/**
 * @brief interní struktura pro předávání všech hodnot z přerušení od SPI
 * do zdroje a vice-versa
 */
static data_t data;

static void autoRefresh(arg_t arg);

/**
 * @brief přerušení že přišly data z SPI od mastera
 *
 * rozpoznání příkazu rovnou tady, odpověď rovnou odsud
 * s tím že odpovědi budou už někde připraveny v paměti
 * a jenom se z ní vytáhnou
 * rozjet jednoduché automat
 * přikaz; dolni byte ; horni byte
 */CH_IRQ_HANDLER(SPI1_IRQHandler)
{
	static uint8_t cmd;
	static uint8_t write;
	static uint16_t dta;
	static uint8_t machine = 0;
	static uint8_t counter = 0;

	CH_IRQ_PROLOGUE();
	//chSysLockFromIsr();
	counter++;

	uint16_t spi = SPI1->DR;
	switch (machine)
	{
	case 0:
		cmd = spi;
		if (cmd & WRITE)
		{
			write = 1;
			cmd &= ~WRITE;
			SPI1->DR = 0;
			machine = 1;
		}
		else
		{
			write = 0;
			SPI1->DR = data.returnVals[cmd] & 0xFF;
			machine = 1;
		}
		break;
	case 1:
		if (write)
		{
			dta = spi;
			machine = 2;
		}
		else
		{
			SPI1->DR = data.returnVals[cmd] >> 8;
			machine = 5; //už není potřeba obsluhovat
		}
		break;
	case 2:
		dta |= spi << 8;
		machine = 0;
		data.writeVals[cmd].changed = TRUE;
		data.writeVals[cmd].value = dta;
		break;
	default:
		machine = 0;
		SPI1->DR = 0;
		break;
	}

	SPI1->SR = 0;
	//machine++;

	//chSysUnlockFromIsr();
	CH_IRQ_EPILOGUE();

}

/**
 * @brief inicializuje SPI1 jako slave
 *
 * @details
 * 	- přemapuje JTAG kvuli SPI chipselectu kterej je na PA15
 * 	- nastaví piny periferie
 * 	- zapne SPI jako slave, jeho hodiny a přerušení od přijatyho bytu
 * 	- inicializuje \ref autoRefresh
 * 	- nastaví metody ktery se maji volat při změně
 */
void remoteInit(void)
{
	static delay_t del;
#if 1
	//přemapování SPI1
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= 1;
	//odpojit JTAG kvuli PA15
	AFIO->MAPR |= 0b010 << 24;
	//mosi, sck
#endif
	//port init
	palSetPadMode(GPIOB,3,PAL_MODE_INPUT);
	palSetPadMode(GPIOB,4,PAL_MODE_STM32_ALTERNATE_PUSHPULL);
	palSetPadMode(GPIOB,5,PAL_MODE_INPUT);
	palSetPadMode(GPIOA,15,PAL_MODE_INPUT);

	//spi slave init, rx interrupt
	rccEnableSPI1(TRUE);
	//SPI1->CR1 = SPI_CR1_DFF   ;
	//priority must be lower than 0
	nvicEnableVector(SPI1_IRQn, CORTEX_PRIORITY_MASK(6));
	SPI1->CR2 = SPI_CR2_RXNEIE;
	SPI1->CR1 = SPI_CR1_SPE;

	SPI1->DR = 0;

	data.writeVals[wCURRENTLIMIT].method = zdrSetCurrentLimit;
	data.writeVals[wOUTPUTVOLTAGE].method = zdrSetVoltage;
	data.writeVals[wENABLE].method = (fce) zdrSetEnabled;

	shFillStruct(&del, autoRefresh, NULL, MS2ST(10), PERIODIC);
	shRegisterStruct(&del);
}

/**
 * @brief funkce volaná periodicky pro přetahování
 * naměřenéch hodnot do struktury \ref data a naopak volání
 * metod kde se změnila hodnota
 */
void autoRefresh(arg_t arg)
{
	(void) arg;
	data.returnVals[rCURRENT] = zdrGetCurrent();
	data.returnVals[rVOLTAGE] = zdrGetVoltage();
	data.returnVals[rCONVERTERVOLTAGE] = zdrGetVoltageConverter();
	data.returnVals[rCURRENTLIMITED] = zdrIsCurrentLimited();
	data.returnVals[rOUTPUTENABLED] = zdrIsOutputEnabled();
	data.returnVals[rTHERMALFAILURE] = zdrIsThermalFailure();

	dataWrite_t * t;
	int i;
	for (i = 0; i < wCOUNT; i++)
	{
		t = &data.writeVals[i];
		if (t->changed)
		{
			t->method(t->value & 0xff);
			t->changed = FALSE;
		}
	}
}

/**
 * @}
 */
