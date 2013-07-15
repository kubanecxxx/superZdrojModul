#include "ch.h"
#include "hal.h"
#include "scheduler.h"
#include "zConverter.h"
#include "zAD.h"

void blik(void * arg)
{
	(void) arg;


	palTogglePort(GPIOC,(1 << 13) | (1 << 14) | (1 <<15));

	uint16_t vol = 150;
	conSetValue(vol);
	uint16_t mereno = conGetVoltage();
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

	conInit();
	adInit();

	while (TRUE)
	{
		shPlay();
		chThdSleepMilliseconds(1);
	}
}

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
