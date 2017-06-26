/*
 * Copyright (C) 2017 Inria
 *               2017 Inria Chile
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_sx127x
 * @{
 * @file
 * @brief       Default configuration for SX127X driver
 *
 * @author      Jos� Ignacio Alamos <jose.alamos@inria.cl>
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef LORA_H
#define LORA_H

#include "net/gnrc/netdev.h"
#include "net/netdev.h"

#include "sx127x_internal.h"
#include "sx127x_params.h"
#include "sx127x_netdev.h"

#define SX127X_LORA_MSG_QUEUE   (16U)
#define SX127X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)

#define MSG_TYPE_ISR            (0x3456)

#define NODE_COUNT (5)
#define NODE_RX		1
#define NODE_TX		0

typedef struct lora_frame {
    uint8_t	nodeID;
	uint8_t frameID;
    char data[6];
} lora_frame ;

typedef enum lora_nodeID
{
    NODEID_NODEone 				= 0x01,
	NODEID_NODEtwo				= 0x02
} lora_nodeID;

typedef enum lora_frameID
{
    FRAMEID_HEARTBEAT 			= 0x01,
	FRAMEID_RFID_TAGID			= 0x02,
	FRAMEID_FENCE_TRANSFERVAL	= 0x10,
} lora_frameID;


int lora_setup_cmd(int argc, char **argv) ;
void lora_init(int listen);
int lora_send_frame(lora_frame payload);
void *_recv_thread(void *arg);

#endif
/** @} */