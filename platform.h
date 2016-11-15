/**
 * @file platform.h
 * @author kubanec
 * @date 8. 4. 2014
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



//#define CORTEX_ENABLE_WFI_IDLE TRUE
#define DEBUG
//#define OPTIMIZE_FINISH
/*
 * vypne uplně všechno jenom bude nastavovat hodnoty na DA převodniku
 * použit stmaster
 */
//#define ADC_DEBUG

#ifdef DEBUG
#define DEBUG_LED
#endif

#ifdef OPTIMIZE_FINISH
#define CH_DBG_SYSTEM_STATE_CHECK FALSE
#define CH_DBG_ENABLE_CHECKS            FALSE
#define CH_DBG_ENABLE_ASSERTS           FALSE
#define CH_DBG_ENABLE_TRACE             FALSE
#define CH_DBG_ENABLE_STACK_CHECK       FALSE
#define CH_DBG_FILL_THREADS             FALSE
#define CH_DBG_THREADS_PROFILING        FALSE
#endif


/*===========================================================================*/
/**
 * @brief špecialni přepočty
 */
/*===========================================================================*/
/// výpočet napětí co je za děličem
#define DELIC(r1,r2,u) ((r2* u)/(r2+r1) )
/// výpočet napětí co je před děličem
#define ODDELIC(r1,r2,u) (((r1+r2)* u) /r2)

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H_ */
