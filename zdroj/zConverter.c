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

#define LDAC_H() spi_high(LDAC)
#define LDAC_L() spi_low(LDAC)

#define spi_high(_pin) palSetPad(spi[_pin].gpio,spi[_pin].pin)
#define spi_low(_pin) palClearPad(spi[_pin].gpio,spi[_pin].pin)
#define spi_setMode(_pin,mode) palSetPadMode(spi[_pin].gpio,spi[_pin].pin,mode)

static uint16_t low_level_spi_in_out(uint16_t cmd);

typedef enum
{
	MOSI, SCK, CS, LDAC
} spi_t;

typedef struct
{
	GPIO_TypeDef * gpio;
	uint8_t pin;
} spi_pin;

static const spi_pin spi[] =
{
{ GPIOB, 7 }, //MOSI
		{ GPIOB, 8 }, //SCK
		{ GPIOB, 9 }, //CS
		{ GPIOB, 6 } //LDAC
};

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
	uint16_t ven;
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

	tisicU = 1230 - ((voltage * 35) / 10);
	data = tisicU * 2;
	/*
	 txbuf[0] = data >> 8;
	 txbuf[1] = data & 0xff;
	 txbuf[0] |= 0b00110000;
	 */
	ven = data;
	ven |= 0b00110000 << 8;

	LDAC_H();
	low_level_spi_in_out(ven);
	LDAC_L();

	return TRUE;
}

/**
 * @brief softwarovy spi
 * 16bitů MSB first
 */
uint16_t low_level_spi_in_out(uint16_t cmd)
{
	uint8_t i;

	spi_low(SCK);
	spi_low(CS);
	for (i = 0; i < 16; i++)
	{
		if (cmd & 0x8000)
			spi_high(MOSI);
		else
			spi_low(MOSI);
		spi_high(SCK);
		cmd <<= 1;
		spi_low(SCK);

	}
	spi_high(CS);
	return 0;
}

/**
 * @brief nastavení SPI a DA převodníku
 */
void conInit(void)
{
	//low level init
	spi_setMode(MOSI,PAL_MODE_OUTPUT_PUSHPULL);
	spi_setMode(SCK,PAL_MODE_OUTPUT_PUSHPULL);
	spi_setMode(LDAC,PAL_MODE_OUTPUT_PUSHPULL);
	spi_setMode(CS,PAL_MODE_OUTPUT_PUSHPULL);
}



/**
 * @brief vrátí naměřeny napěti na vystupu měniče
 * hodnota bude v mV at to nemusi byt float
 */
uint16_t conGetVoltage(void)
{
	//přepočitat adcdata děličem a vrátit
	uint32_t temp = ODDELIC(zConstants.measConvVoltR1,zConstants.measConvVoltR2,conAdcData);

	return temp;
}

/**
 * @}
 */
