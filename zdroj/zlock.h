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

/**
 * @ingroup zlock
 * @{
 */

/**
 * @brief sada konstant pro trimrování zdroje
 */
typedef struct zConstants_t zConstants_t;

/**
 * @brief sada konstant pro trimrování zdroje
 */
struct zConstants_t
{
	uint16_t measCurMul;///< měření proudu násobení
	uint16_t measCurDiv;///< měření proudu dělení
	uint16_t measOutVoltR1;///< odpor R1 při měření výstupního napětí
	uint16_t measOutVoltR2;///< odpor R2 pro měření výstupního napětí
	uint16_t measConvVoltR1;///< odpor R1 děliče  měření napětí za měničem
	uint16_t measConvVoltR2;///< odpor R2 děliče měření napětí za měničem

	uint16_t opaVoltR1;///< vstupní odpor OPA
	uint16_t opaVoltR2;///< feedback odpor OPA

	/// todo opa limit current
};

extern const zConstants_t zConstants;

/**
 * @}
 */

#endif /* LOCK_H_ */
