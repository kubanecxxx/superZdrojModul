#include "ch.h"
#include "hal.h"
#include "scheduler.h"
#include "zdroj.h"
#include "remote.h"

//#pragma GCC push_options
//#pragma GCC optimize ("O0")

uint16_t mv = 10000;
uint16_t ma = 100;
bool_t en = TRUE;
uint16_t mereni;
bool_t boleny;

void blik(void * arg)
{
	(void) arg;
	palTogglePort(GPIOC,(1 << 13) | (1 << 14) );

	zdrSetEnabled(en);
	zdrSetVoltage(mv);
	zdrSetCurrentLimit(ma);
	mereni = zdrGetCurrent();
	mereni = zdrGetVoltage();
	mereni = zdrGetVoltageConverter();
	boleny = zdrIsOutputEnabled();
	boleny = zdrIsThermalFailure();
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
	//shRegisterStruct(&blikej);

	zdrInit();

	remoteInit();

	while (TRUE)
	{
		shPlay();
		chThdSleepMilliseconds(1);
		zdrProcessData();
	}

	return 1;
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
