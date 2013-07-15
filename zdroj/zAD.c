/**
 * @file zAD.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#include "zAD.h"
#include "ch.h"
#include "hal.h"

static void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);
/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS  (4 + 1)

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      2

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
 */ADC_CR2_TSVREFE,
/*
 * smpr1
 */
ADC_SMPR1_SMP_VREF(SAMPLE_TIME), /*
 * smpr2
 */ADC_SMPR2_SMP_AN0(SAMPLE_TIME) | ADC_SMPR2_SMP_AN1(SAMPLE_TIME)
		| ADC_SMPR2_SMP_AN2(SAMPLE_TIME) | ADC_SMPR2_SMP_AN3(SAMPLE_TIME), /*
 * sqr1
 */ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS), /*
 * sqr2
 */0, /*
 * sqr3
 */ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN1)
		| ADC_SQR3_SQ3_N(ADC_CHANNEL_IN2) | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN3) | ADC_SQR3_SQ5_N(ADC_CHANNEL_VREFINT) };

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

/**
 * @brief callback od virtuálního timeru dycky jenom spustí další adcpřevod
 */
void vtcb(void * arg)
{
	(void) arg;

	chSysLockFromIsr();
	adcStartConversionI(&ADCD1, &adcgrpcfg, bufer, ADC_GRP1_BUF_DEPTH);
	chSysUnlockFromIsr();
}

extern uint16_t conAdcData;

/**
 * @brief callback od hotovyho adc převodu, jenom znova nahodi virtualni timer
 * na dalši spuštění převodu
 * a nahází změřeny data do přislušnéch proměnnéch od modulů
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
	(void) n;
	uint16_t buf[4];

	if (adcp->state != ADC_COMPLETE)
		return;


	int i;
	for(i = 0 ;  i < 4 ;i ++)
	{
		buf[i] = buffer[i] * 1200 / buffer[4];
	}


	conAdcData = buf[0]; //napěti za měničem
	/// todo dalši napěti pro opa

	chSysLockFromIsr();
	if (!chVTIsArmedI(&vt))
		chVTSetI(&vt, MS2ST(50), vtcb, NULL);
	chSysUnlockFromIsr();
}

//#pragma GCC pop_options


