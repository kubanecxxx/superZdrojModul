#include "ch.h"
#include "hal.h"
#include "scheduler.h"
#include "zdroj.h"
#include "remote.h"

zdr_t zdr ={.ma = 100, .mv = 10000, .en = TRUE};
systime_t sysTime;

void blik(void * arg)
{
	(void) arg;
	palTogglePort(GPIOC,(1 << 13) | (1 << 14) );

	zdrSetup(&zdr);
}

delay_t blikej;

int main(void)
{
	chSysInit();
	halInit();

	chRegSetThreadName("Main");

	palSetGroupMode(GPIOC,0b111,13,PAL_MODE_OUTPUT_PUSHPULL);
	palClearPort(GPIOC,(1 << 13) | (1 << 14) | (1 <<15));

#ifndef ADC_DEBUG
	shFillStruct(&blikej, blik, NULL, (200), PERIODIC);
	shRegisterStruct(&blikej);

	zdrInit();
	remoteInit();

	while(TRUE)
	{
        sysTime = chVTGetSystemTime();
		shPlay();
		chThdSleepMilliseconds(1);
		zdrProcessData();
	}
#else
	zdrInit();
	remoteInit();
	while(TRUE)
	{
		zdrDacRoutine();
		palTogglePad(GPIOC,13);
		palTogglePad(GPIOC,14);
		chThdSleepMilliseconds(100);
	}
#endif

	return 1;
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

