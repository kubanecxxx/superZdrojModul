#include "ch.h"
#include "hal.h"

int main(void)
{
	halInit();
	chSysInit();
	chRegSetThreadName("Main");

	while (TRUE)
	{
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
