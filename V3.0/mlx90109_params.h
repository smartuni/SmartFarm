/*
 * animaltag.h
 *
 * Created: 06.04.2017 17:44:02
 *  Author: Jonas Radtke
 */ 

#include "board.h"  /* THIS INCLUDE IS MANDATORY */
#include "mlx90109.h"


/**
 * @brief   Default configuration parameters for TMPABC sensors
 * @{
 */
#ifndef MLX90109_PARAM_CLOCK
#define MLX90109_PARAM_CLOCK      (0)//(GPIO_PIN(PORT_E, 2))
#endif
#ifndef MLX90109_PARAM_DATA
#define MLX90109_PARAM_DATA       (0)//(GPIO_PIN(PORT_D, 7))
#endif
#ifndef MLX90109_PARAM_MODE
#define MLX90109_PARAM_MODE 	  (0)
#endif
#ifndef MLX90109_PARAM_DATASELECT
#define MLX90109_PARAM_DATASELECT (0)
#endif
#ifndef MLX90109_PARAM_MODU
#define MLX90109_PARAM_MODU       (0)
#endif
#ifndef MLX90109_PARAM_HANDLER
#define MLX90109_PARAM_HANDLER    interrupt_hand
#endif
#ifndef MLX90109_PARAM_SPEED
#define MLX90109_PARAM_SPEED      (4000)
#endif

#ifndef MLX90109_PARAMS
#define MLX90109_PARAMS           { .clock			= MLX90109_PARAM_CLOCK ,\
									.data			= MLX90109_PARAM_DATA ,\
									.mode			= MLX90109_PARAM_MODE ,\
									.dataSelect		= MLX90109_PARAM_DATASELECT ,\
									.modu			= MLX90109_PARAM_MODU ,\
									.handler		= MLX90109_PARAM_HANDLER, },
#endif
/** @} */

/**
 * @brief   Allocation of TMPABC configuration
 */
 
