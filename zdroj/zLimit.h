/**
 * @file zLimit.h
 * @author kubanec
 * @date 28.8.2013
 *
 */

#ifndef ZLIMIT_H_
#define ZLIMIT_H_

#include "zlock.h"

void zLimInit(void);
inline bool zLimIsCurrentLimited(void);

#endif /* ZLIMIT_H_ */
