/**
 * @file zOpa.c
 * @author kubanec
 * @date 14.7.2013
 *
 */

#include "ch.h"
#include "hal.h"
#include "zOpa.h"
#include "zDA.h"

/**
 * @ingroup zdroj
 * @defgroup opa
 * @brief API pro ovládání OPA548 interním DA převodníkem
 * @{
 */

/**
 * @brief inicializace zdroje OPA + init DA převodníků
 */
void opaInit(void)
{
	daInit();
}

/*
 * @}
 */
