/**
 * @file zlock.c
 * @author kubanec
 * @date 28.8.2013
 *
 */

#define _ZDROJ_SUBMODULES

#include "zlock.h"

/**
 * @ingroup zdroj
 * @defgroup zlock
 * @brief zamykání at se nedá inkludovat + sada konstant
 * @{
 */

/**
 * @brief sada konstant pro trimrování zdroje ať sou na jednom místě
 */
const zConstants_t zConstants =
{
	.measCurMul = 11,
	.measCurDiv = 20,
	.measOutVoltR1 = 33000,
	.measOutVoltR2 = 10000, //přepájet na 1200
	.opaVoltR1 = 1000,
	.opaVoltR2 = 10000,
	.measConvVoltR1 = 33000,
	.measConvVoltR2 = 1200
};

/**
 * @}
 */
