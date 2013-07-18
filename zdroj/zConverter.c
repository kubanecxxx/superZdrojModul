/**
 * @file zConverter.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "ch.h"
#include "hal.h"
#include "zConverter.h"

#define LDAC_PORT GPIOA
#define LDAC_PIN 15

#define LDAC_H() palSetPad(LDAC_PORT,LDAC_PIN)
#define LDAC_L() palClearPad(LDAC_PORT,LDAC_PIN)

#define CS_PORT GPIOB
#define CS_PIN 4

static const SPIConfig ls_spicfg =
{ NULL, CS_PORT, CS_PIN, 0 };

//napětí už bude v mV
uint16_t conAdcData = 0;

/**
 * @ingroup zdroj
 * @defgroup měnič
 * @brief měnič pro předregulaci napětí
 * @{
 */

/**
 * @brief nastavit vystupní napětí na výstupu měniče
 * posílá se hodnota 10x větší než se má ve skutečnosti nastavit
 */
bool_t conSetVoltage(uint16_t voltage)
{
	uint8_t txbuf[2];
	uint8_t rxbuf[2];
	uint32_t tisicU;
	uint16_t data;

	if (voltage < 100 || voltage > 350)
		return FALSE;

	/**
	 * nejdřiv vypočitat kolik V má lizt z DA převodniku
	 * podle 1.23V a děliče
	 * tohle napěti ještě přepočitat na data ktery mam poslat
	 * do DA převodniku
	 */

	tisicU = 1230 - ((voltage*35) / 10);
	data = tisicU * 2;
	txbuf[0] = data >> 8;
	txbuf[1] = data & 0xff;
	txbuf[0] |= 0b00110000;

	LDAC_H();
	spiSelect(&SPID1);
	spiExchange(&SPID1,2,txbuf,rxbuf);
	spiUnselect(&SPID1);
	LDAC_L();

	return TRUE;
}

/**
 * @brief nastavení SPI a DA převodníku
 */
void conInit(void)
{
	//přemapování SPI1
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= 1;
	//odpojit JTAG kvuli PA15
	AFIO->MAPR |= 0b010 << 24;
	//mosi, sck
	palSetGroupMode(GPIOB,0b101 , 3, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
	//cs
	palSetPadMode(CS_PORT,CS_PIN,PAL_MODE_OUTPUT_PUSHPULL);
	//ldac
	palSetPadMode(LDAC_PORT,LDAC_PIN,PAL_MODE_OUTPUT_PUSHPULL);

	spiStart(&SPID1, &ls_spicfg);
	spiUnselect(&SPID1);
}

#define R2	1200
#define R1	33000

/**
 * @brief vrátí naměřeny napěti na vystupu měniče
 * hodnota bude v mV at to nemusi byt float
 */
uint16_t conGetVoltage(void)
{
	//přepočitat adcdata děličem a vrátit
	uint32_t temp = ODDELIC(R1,R2,conAdcData);

	return temp;
}

/**
 * @}
 */
