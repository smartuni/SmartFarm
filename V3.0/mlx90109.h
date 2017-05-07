/*
 * animaltag.h
 *
 * Created: 06.04.2017 17:44:02
 *  Author: Jonas Radtke
 */ 


#ifndef MLX90109_H_
#define MLX90109_H_

#include <stdint.h>
#include <stdio.h>
#include "periph/gpio.h"

typedef struct {
    gpio_t clock;          /**<  */
    gpio_t data;   		   /**<  */
	gpio_t mode;
	gpio_t dataSelect;
	gpio_t modu;
	gpio_cb_t handler;
} mlx90109_params_t;

typedef struct {
    mlx90109_params_t p;  /**< device configuation parameter */
	gpio_cb_t handler;
} mlx90109_t;

typedef struct {
	uint64_t tagId;				// Tag ID
	uint16_t countryCode;		// Country Code of TaG
	uint16_t dataBlock;			// Data Block is used
	uint16_t animalTag;			// For Animal Identification
	uint8_t checksumData[8];	// 64bit for chemsum calculation
	uint16_t checksumArr[2];	// Checksum 2byte of 64bit block
	uint16_t checksum16;		// 16bit checksum
	uint8_t dataB[3];			// for Data on Tag, if DataBlock is 1
	uint8_t counter;
	uint8_t counter_header;
	uint8_t data[128];
} tagdata;

void animaltag_init(mlx90109_t *, const mlx90109_params_t *);
void fdx_format(mlx90109_t *, tagdata *);
void fdxRead(mlx90109_t *, tagdata *);

#endif /* MLX90109_H_ */