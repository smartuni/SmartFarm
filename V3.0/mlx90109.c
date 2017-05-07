/*
 * animaltag.c
 *
 * Created: 06.04.2017 17:43:46
 *  Author: Jonas Radtke
 */ 

 
/*
* 
*  10000000000 	  11bit Header
*  1 11110000 	
*  1 ->00000011 	  38 bit, 12 digits, ID.
*  1 00000000 	  eg. 000000001008 (decimal).
*  1 00000000 	
*  1 11->000000   10 bit (3 digit) Country code.
*  1 11111001 	  eg. 999 (decimal).
*  1 -------1 	  1 bit data block status flag.
*  1 1------- 	  1 bit animal application indicator.
*  1 11010110 	  16 bit checksum.
*  1 01011101 	
*  1 01010110 	  24 bits of extra data if present.
*  1 00110100 	 
*  1 00010010 	
*
*/
 
 
 
 
 
#include "mlx90109.h"
#include <string.h>


void animaltag_init(mlx90109_t *dev, const mlx90109_params_t *params)
{
	int error = 0;
	
	/* write config params to device descriptor */
	memcpy(&dev->p, params, sizeof(mlx90109_params_t));
	
	//Init data pin
	error = gpio_init( dev->p.data,GPIO_IN_PU);
	if(error<0) printf("Fehler");
	//Init Clock Pin and ISR
	gpio_init_int( dev->p.clock, GPIO_IN_PU , GPIO_RISING , dev->p.handler , NULL);
	gpio_irq_enable(dev->p.clock);
	
	//Init mode Pin if used
	if (dev->p.mode){
		error =	gpio_init( dev->p.mode,GPIO_IN_PU); 	
	}
	//Init data Rate Pin if used
	if (dev->p.dataSelect){
		error = gpio_init( dev->p.dataSelect,GPIO_IN_PU); 	
	}
	
	//Init Modu Pin if used
	if (dev->p.modu){
		error =	gpio_init( dev->p.modu,GPIO_IN_PU); 	
	}
	
	if(error){}
	
}

void fdx_format(mlx90109_t *dev, tagdata *tag)
{
	if(dev){}
	uint8_t i=0;
	uint8_t k=0;
//	tag = 0;
	
	// Data for Checksum
	for (k=0; k<=7; k++)
	{
		tag->checksumData[k] = 0;
		for (i=(k*9); i<=7+(k*9); i++)
		{
			tag->checksumData[k] |= (tag->data[i] << (i-(k*9)));
		}
	}

	// Checksum format
	tag->checksumArr[0] = 0; tag->checksumArr[1] = 0;
	for (i=72; i<=79; i++)
	{
		tag->checksumArr[0] |= (tag->data[i] << (i-72));
	}
	
	for (i=81; i<=88; i++)
	{
		tag->checksumArr[1] |= (tag->data[i] << (i-81));
	}
	
	tag->checksum16 = 0;
	tag->checksum16 = tag->checksum16 | tag->checksumArr[0] | (uint16_t)tag->checksumArr[1] << 8;
	
	// Tag ID format
	tag->tagId = 0;
	tag->tagId = tag->tagId | (uint64_t)tag->checksumData[0] | (uint64_t)tag->checksumData[1] << 8 | (uint64_t)tag->checksumData[2] << 16 | (uint64_t)tag->checksumData[3] << 24 | (uint64_t)(tag->checksumData[4] & 0x3F) << 32 ;
	
	// Tag Countrycode format
	tag->countryCode = 0;
	tag->countryCode = tag->countryCode | ((uint16_t)tag->checksumData[5] << 2) | ((tag->checksumData[4] & 0xC0) >> 6);
	
	// Tag Data Block used and Tag for Animal Identification
	tag->dataBlock = 0;
	tag->dataBlock = tag->checksumData[6] & 0x01;
	tag->animalTag = 0;
	tag->animalTag = (tag->checksumData[7] & 0x80) >> 7;
	
	// Datablock
	for (i=90; i<=97; i++)
	{
		tag->dataB[0] |= (tag->data[i] << (i-90));
	}
	
	for (i=99; i<=106; i++)
	{
		tag->dataB[1] |= (tag->data[i] << (i-99));
	}
	
	for (i=108; i<=115; i++)
	{
		tag->dataB[2] |= (tag->data[i] << (i-108));
	}
	
	return;
}


void fdxRead(mlx90109_t *dev, tagdata *tag)
{
			
	if((gpio_read(dev->p.data) > 0)&&(tag->counter_header==11))
	{
		tag->data[tag->counter]=1;
		tag->counter++;
		//uartsendbyte ('1');
	}
	
	if ((!(gpio_read(dev->p.data)))&&(tag->counter_header==11))
	{
		tag->data[tag->counter]=0;
		tag->counter++;
		//uartsendbyte ('0');
		
	}
	
	if ((!(gpio_read(dev->p.data)))&&(tag->counter_header<10))
	{
		tag->counter_header++;
	}
	
	if ((gpio_read(dev->p.data) > 0)&&(tag->counter_header<10))
	{
		tag->counter_header=0;
		tag->counter = 0;
	}
	
	if ((gpio_read(dev->p.data) > 0) && (tag->counter_header == 10))
	{
		tag->counter_header++;
	}


	if ( tag->counter > 127)
	{
	//	tag->counter = 0;
	//	tag->counter_header = 0;		
	//	daten = fdx_format(data);
		
	}

	return;
}
