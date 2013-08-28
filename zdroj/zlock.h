/**
 * @file lock.h
 * @author kubanec
 * @date 17.7.2013
 *
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifndef _ZDROJ_SUBMODULES
#error "zfiles can be included only in zdroj.c"
#endif

#include "stdint.h"

typedef struct zConstants_t zConstants_t;
struct zConstants_t
{
	uint16_t measCurMul;
	uint16_t measCurDiv;
	uint16_t measOutVoltR1;
	uint16_t measOutVoltR2;
	uint16_t measConvVoltR1;
	uint16_t measConvVoltR2;

	uint16_t opaVoltR1;
	uint16_t opaVoltR2;

	/// todo opa limit current
};

extern const zConstants_t zConstants;

#endif /* LOCK_H_ */
