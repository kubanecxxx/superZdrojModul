/**
 * @file zlock.c
 * @author kubanec
 * @date 28.8.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "zlock.h"

const zConstants_t zConstants =
{
	.measCurMul = 11,
	.measCurDiv = 20,
	.measOutVoltR1 = 33000,
	.measOutVoltR2 = 10000,//přepájet na 1200
	.opaVoltR1 = 1000,		//vstupní
	.opaVoltR2 = 10000,		//feedback
	.measConvVoltR1 = 33000,
	.measConvVoltR2 = 1200
};
