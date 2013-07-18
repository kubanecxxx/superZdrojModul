/**
 * @file zAD.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "zAD.h"
#include "ch.h"
#include "hal.h"
#include <string.h>
#include "ringBuffer.h"

/**
 * @ingroup zdroj
 * @defgroup AD
 * @brief AD převodnik pro měření výstupních napětí
 * @{
 */

static void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);
/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS  (4 + 1)

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      20

/*
 * ADC samples buffer.
 */
static adcsample_t bufer[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

#define SAMPLE_TIME ADC_SAMPLE_239P5

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 4 samples of 2 channels, SW triggered.
 * Channels:    IN11   (48 cycles sample time)
 *              Sensor (192 cycles sample time)
 */
static const ADCConversionGroup adcgrpcfg =
{ /*
 * circular
 */
FALSE, /*
 * počet kanálů
 */ADC_GRP1_NUM_CHANNELS,
/*
 * hotovo callback
 */
adccb, /*
 * error callback
 */NULL,
/* HW dependent part.*/
/*
 * CR1
 */
0, /*
 * CR2
 */ADC_CR2_TSVREFE, /*
 * smpr1
 */ADC_SMPR1_SMP_VREF(SAMPLE_TIME), /*
 * smpr2
 */ADC_SMPR2_SMP_AN0(SAMPLE_TIME) | ADC_SMPR2_SMP_AN1(SAMPLE_TIME)
		| ADC_SMPR2_SMP_AN2(SAMPLE_TIME) | ADC_SMPR2_SMP_AN3(SAMPLE_TIME), /*
 * sqr1
 */ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS), /*
 * sqr2
 */0, /*
 * sqr3
 */ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN1)
		| ADC_SQR3_SQ3_N(ADC_CHANNEL_IN2) | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN3)
		| ADC_SQR3_SQ5_N(ADC_CHANNEL_VREFINT) };

//#pragma GCC push_options
//#pragma GCC optimize ("O2")

static VirtualTimer vt;
static void vtcb(void *);

/**
 * @brief inicializuje AD převodnik aby sám vzorkoval
 * s nějakou časovou periodou všechny vstupy ktery sou potřeba
 * a vysledky házel do proměnnéch od přislušnéch modulu
 */
void adInit(void)
{
	palSetGroupMode(GPIOA,0b1111,0,PAL_MODE_INPUT_ANALOG);
	adcStart(&ADCD1, NULL);
	chVTSet(&vt,MS2ST(50),vtcb,NULL);
}

static bool_t buffer_full = FALSE;

/**
 * @brief callback od virtuálního timeru dycky jenom spustí další adcpřevod
 *
 * nechá ukládat data o kousek dál do bufferu, až bude buffer plné
 * tak data nechá schroustat a zase začne ukládat od začátku
 */
void vtcb(void * arg)
{
	(void) arg;
	static uint16_t * ptr = bufer;

	chSysLockFromIsr();
	adcStartConversionI(&ADCD1, &adcgrpcfg, ptr, 2);
	chSysUnlockFromIsr();

	//pokaždy si inkrementuje pointer
	//až je buffer plnej tak to vrátí na začátek a řekne to někomu
	ptr += ADC_GRP1_NUM_CHANNELS * 2;
	if (ptr == bufer + ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH)
	{
		ptr = bufer;
		buffer_full = TRUE;
	}
}

extern uint16_t conAdcData;
extern uint16_t _adNapeti;
extern uint16_t _adProud1, _adProud2;

/**
 * @brief callback od hotovyho adc převodu, jenom znova nahodi virtualni timer
 * na dalši spuštění převodu
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
	(void) n;
	(void) buffer;

	if (adcp->state != ADC_COMPLETE)
		return;

	chSysLockFromIsr();
	if (!chVTIsArmedI(&vt) && buffer_full == FALSE)
		chVTSetI(&vt, MS2ST(10), vtcb, NULL);
	chSysUnlockFromIsr();
}

/**
 * @brief zchroustá data z AD převodníku
 *
 * 10 dvouvzorků po 10ms => každéch 100ms zpruměruje vzorky
 *
 */
void adProcessData(void)
{
	uint16_t buf[ADC_GRP1_NUM_CHANNELS];
	memset(buf, 0, sizeof(buf));

	if (buffer_full == FALSE)
		return;

	buffer_full = FALSE;

	if (!chVTIsArmedI(&vt))
		chVTSet(&vt, MS2ST(10), vtcb, NULL);

	int j, i;
	for (i = 0; i < ADC_GRP1_NUM_CHANNELS; i++)
	{
		for (j = 0; j < ADC_GRP1_BUF_DEPTH; j++)
		{
			buf[i] += bufer[i + j * ADC_GRP1_NUM_CHANNELS];
		}
		buf[i] /= ADC_GRP1_BUF_DEPTH;

	}

	for (i = 0; i < 4; i++)
	{
		buf[i] = buf[i] * 1200 / buf[4];
	}

	 conAdcData = buf[0]; //napěti za měničem
	 _adProud2 = buf[1];
	 _adProud1 = buf[2];
	 _adNapeti = buf[3];
}

//#pragma GCC pop_options

/**
 * @}
 */

