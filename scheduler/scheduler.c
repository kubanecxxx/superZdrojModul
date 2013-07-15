/*
 * scheduler.c
 *
 *  Created on: 11.4.2013
 *      Author: kubanec
 */

#include "scheduler.h"
#include "string.h"
#include "ch.h"

static void shRunMethod(delay_t * method);

//volatile systime_t sysTime = 0;
#define sysTime chTimeNow()
static delay_t * first = NULL;
static delay_t * last = NULL;

void shInit(void)
{
	_shLowLevelInit();
}

systime_t shNow(void)
{
	return sysTime;
}

void shPlay(void)
{
	//projet celej seznam
	//zavolat shRunMethod
	delay_t * del = first;
	while (del)
	{
		shRunMethod(del);
		del = del->next;
	}
}

void shFillStruct(delay_t * del, method_t method, arg_t arg, systime_t cycles,
		sh_t type)
{
	del->method = method;
	del->arg = arg;
	del->type = type;
	del->next = NULL;
	del->period = cycles;
	del->last = 0;
}

/*
 * nastavit posledni pointer na tuhle strukturu
 */
void shRegisterStruct(delay_t * del)
{
	if (shIsRegistered(del))
		return;

	if (first == NULL)
		first = del;
	if (last)
		last->next = del;
	last = del;
	del->last = sysTime;
}

uint8_t shIsRegistered(delay_t * del)
{
    if (del == first)
    {
        return 1;
    }

    delay_t * temp = first;
    while (temp)
    {
        if (temp->next == del)
        {
            return 1;
        }
        temp = temp->next;
    }

    return 0;
}

void shUnregisterStruct(delay_t * del)
{
	if (!shIsRegistered(del))
		return;

	delay_t * temp = first;

	if (del == first)
	{
		first = del->next;
		return;
	}

	while (temp)
	{
		if (temp->next == del)
		{
			temp->next = del->next;
			if (del == last)
				last = temp;

			return;
		}
		temp = temp->next;
	}
}

void shRunMethod(delay_t * method)
{
	if (sysTime - method->last > method->period)
	{
		if (method->type == ONCE)
					shUnregisterStruct(method);

		method->last = sysTime;
		method->method(method->arg);
	}
}

void shRearm(delay_t * del)
{
	del->last = sysTime;
}

static void seqCb(arg_t arg)
{
    sequencer_t * seq = (sequencer_t*)arg;
    seqItem_t item;
    memcpy(&item,seq->items+seq->actualState,sizeof(seqItem_t));

    seq->cb(item.arg);
    seq->actualState++;
    memcpy(&item,seq->items+seq->actualState,sizeof(seqItem_t));
    if(item.cycles)
    {
        shFillStruct(seq->del,seqCb,seq,item.cycles,ONCE);
        shRegisterStruct(seq->del);
    }
    else
    {
    	if (seq->endCb)
    		seq->endCb(NULL);
    }
}

void seqPlay(sequencer_t *sequencer, delay_t *delay, const seqItem_t *items, method_t cb, method_t end)
{
    sequencer->actualState = 0;
    sequencer->cb = cb;
    sequencer->del = delay;
    sequencer->items = items;
    sequencer->endCb = end;

    shFillStruct(delay,seqCb,sequencer,items->cycles,ONCE);
    shRegisterStruct(delay);
}

void timeoutRearm(systime_t * timeout, systime_t time)
{
	*timeout = sysTime + time;
}

uint8_t timeoutReached(const systime_t * timeout)
{
	return (*timeout < sysTime);
}

