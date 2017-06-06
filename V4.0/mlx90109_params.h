/*
 * Copyright (C) 2017 Jonas Radtke <jonas.radtke@haw-hamburg.de> <jonas@radtke.dk>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

 /**
 * @ingroup     drivers_MLX90109
 * @{
 *
 * @file
 * @brief       Default parameters for MLX90109 driver
 *
 * @author      Jonas Radtke <jonas.radtke@haw-hamburg.de> <jonas@radtke.dk>
 */

#ifndef MLX90109_PARAMS_H
#define MLX90109_PARAMS_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Set default configuration parameters for the MLX90109 driver
 * @{
 */
#ifndef MLX90109_PARAM_CLOCK
#define MLX90109_PARAM_CLOCK       (GPIO_PIN(PORT_E, 2)) // (GPIO_PIN(PORT_A, 15))
#endif
#ifndef MLX90109_PARAM_DATA
#define MLX90109_PARAM_DATA       (GPIO_PIN(PORT_D, 7))//(GPIO_PIN(PORT_C, 14))
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
#ifndef MLX90109_PARAM_SPEED
#define MLX90109_PARAM_SPEED      (4000)
#endif
/** @} */

/**
 * @brief   MLX90109 configuration
 */
#ifndef MLX90109_PARAMS
#define MLX90109_PARAMS           { .clock			= MLX90109_PARAM_CLOCK ,\
									.data			= MLX90109_PARAM_DATA ,\
									.mode			= MLX90109_PARAM_MODE ,\
									.dataSelect		= MLX90109_PARAM_DATASELECT ,\
									.modu			= MLX90109_PARAM_MODU ,},
#endif

static const mlx90109_params_t mlx90109_params[] ={
	MLX90109_PARAMS
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDCARD_SPI_PARAMS_H */
/** @} */
 
