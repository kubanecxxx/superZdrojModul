#include "ch.h"
#include "hal.h"
#include "scheduler.h"
#include "zdroj.h"
#include "zConverter.h"
#include "zAD.h"
#include "zDA.h"

//#pragma GCC push_options
//#pragma GCC optimize ("O0")

void blik(void * arg)
{
	(void) arg;
	palTogglePort(GPIOC,(1 << 13) | (1 << 14) | (1 <<15));

	uint16_t i = 500;
	daSetVoltage(2,i);
}

delay_t blikej;

int main(void)
{
	chSysInit();
	halInit();

	chRegSetThreadName("Main");

	palSetGroupMode(GPIOC,0b111,13,PAL_MODE_OUTPUT_PUSHPULL);
	palClearPort(GPIOC,(1 << 13) | (1 << 14) | (1 <<15));
	shFillStruct(&blikej, blik, NULL, (200), PERIODIC);
	shRegisterStruct(&blikej);

	zdrInit();

	while (TRUE)
	{
		shPlay();
		chThdSleepMilliseconds(1);
	}
}

//#pragma GCC push_options

#ifdef __cplusplus
void* operator new(size_t sz)
{
	void * temp;
	temp = chCoreAlloc(sz);
	if (temp == NULL)
	asm ("bkpt");

	return temp;
}

void operator delete(void* m)
{
	(void) m;
}

#endif
