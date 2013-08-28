/**
 * @file zDA.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "ch.h"
#include "hal.h"
#include "zDA.h"

/**
 * @ingroup zdroj
 * @defgroup DA
 * @brief integrovaný DA převodník pro nastavení výstupního napětí a proudového omezení
 * @{
 */

/**
 * @brief inicializace DA převodníku
 */
void daInit(void)
{
	palSetGroupMode(GPIOA,0b11,4,PAL_MODE_INPUT_ANALOG);

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	DAC->CR |= DAC_CR_TSEL2_0 | DAC_CR_TSEL2_1 | DAC_CR_TSEL2_2;
	DAC->CR |= DAC_CR_TSEL1_0 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_2;
	DAC->CR |= DAC_CR_BOFF1 | DAC_CR_BOFF2;
	DAC->CR |= DAC_CR_EN1 | DAC_CR_EN2;
#if 0
	daSetVoltage(2,500);
	daSetVoltage(1,800);
#endif
}

/**
 * @brief nastavení výstupního napětí na DA převodníku
 */
void daSetVoltage(uint8_t channel, uint16_t mv)
{
	uint32_t data;

	if (channel > 2)
		return;

	data = (uint32_t)  (mv *  1221) / 1000;
	if (channel == 1)
	{
		DAC->DHR12R1 = data;
	}
	else if (channel == 2)
	{
		DAC->DHR12R2 = data;
	}

	//napráší to tam
	DAC->SWTRIGR |= channel;
}

/**
 * @}
 */
