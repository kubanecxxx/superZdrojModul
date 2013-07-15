/*
 * scheduler.h
 *
 *  Created on: 11.4.2013
 *      Author: kubanec
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stddef.h>
#include "scheduler_port.h"

typedef enum
{
	ONCE, PERIODIC
} sh_t;
typedef uint32_t systime_t;
typedef void * arg_t;
typedef void (*method_t)(arg_t);

typedef struct delay_t delay_t;

struct delay_t
{
	method_t method;
	arg_t arg;
	systime_t period;
	systime_t last;
	sh_t type;
	delay_t * next;
};

void shFillStruct(delay_t * del, method_t method, arg_t arg, systime_t cycles,
		sh_t type);
void shRegisterStruct(delay_t * del);
void shUnregisterStruct(delay_t * del);
void shPlay(void);
inline void shInit(void);
void shRearm(delay_t * del);
uint8_t shIsRegistered(delay_t * del);
systime_t shNow(void);

typedef struct sequencer_t sequencer_t;

typedef struct
{
    arg_t arg;
    uint16_t cycles;
} seqItem_t;

struct sequencer_t
{
    uint8_t actualState;
    method_t cb;
    delay_t * del;
    const seqItem_t * items;
    method_t endCb;
};

void seqPlay(sequencer_t * sequencer, delay_t * delay, const seqItem_t * items, method_t cb, method_t endCb);

void timeoutRearm(systime_t * timeout, systime_t time);
uint8_t timeoutReached(const systime_t * timeout);

#ifdef __cplusplus
}
#endif

#endif /* SCHEDULER_H_ */
