#include "lora.h"

static char stack[SX127X_STACKSIZE];
static kernel_pid_t _recv_pid;


static int heartbeat_count[NODE_COUNT] = {0, 0 ,0 ,0 ,0};

static char message[32];
static sx127x_t sx127x;
static netdev_t *netdev;

static void _event_cb(netdev_t *dev, netdev_event_t event);

int lora_setup_cmd(int argc, char **argv) {

    if (argc < 4) {
        puts("usage: setup "
             "<bandwidth (125, 250, 500)> "
             "<spreading factor (7..12)> "
             "<code rate (5..8)>");
        return -1;
    }

    /* Check bandwidth value */
    int bw = atoi(argv[1]);
    uint8_t lora_bw;
    switch (bw) {
        case 125:
            puts("setup: setting 125KHz bandwidth");
            lora_bw = SX127X_BW_125_KHZ;
            break;

        case 250:
            puts("setup: setting 250KHz bandwidth");
            lora_bw = SX127X_BW_250_KHZ;
            break;

        case 500:
            puts("setup: setting 500KHz bandwidth");
            lora_bw = SX127X_BW_500_KHZ;
            break;

        default:
            puts("[Error] setup: invalid bandwidth value given, "
                 "only 125, 250 or 500 allowed.");
            return -1;
    }

    /* Check spreading factor value */
    uint8_t lora_sf = atoi(argv[2]);
    if (lora_sf < 7 || lora_sf > 12) {
        puts("[Error] setup: invalid spreading factor value given");
        return -1;
    }

    /* Check coding rate value */
    int cr = atoi(argv[3]);;
    if (cr < 5 || cr > 8) {
        puts("[Error ]setup: invalid coding rate value given");
        return -1;
    }
    uint8_t lora_cr = (uint8_t)(cr - 4);

    /* Configure radio device */
    netdev_t *netdev = (netdev_t*) &sx127x;
    netdev->driver->set(netdev, NETOPT_LORA_BANDWIDTH,
                        &lora_bw, sizeof(uint8_t));
    netdev->driver->set(netdev, NETOPT_LORA_SPREADING_FACTOR,
                        &lora_sf, 1);
    netdev->driver->set(netdev, NETOPT_LORA_CODING_RATE,
                        &lora_cr, sizeof(uint8_t));

    puts("[Info] setup: configuration set with success");

    return 0;
}


void lora_init(int listen)
{

    memcpy(&sx127x.params, sx127x_params, sizeof(sx127x_params));
    netdev = (netdev_t*) &sx127x;
    netdev->driver = &sx127x_driver;
    netdev->driver->init(netdev);
    netdev->event_callback = _event_cb;

     _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, _recv_thread, NULL,
                        "recv_thread");

   if (_recv_pid <= KERNEL_PID_UNDEF) {
          puts("Creation of receiver thread failed");
            //return 1;
     }

	/* setup for Lora shield 
		bandwidth = 125 kHz
		spreading factor = 12
		code rate = 8
	*/
	char *setup[] = {"setup", "125", "12", "8"};
	lora_setup_cmd(4, setup);
	
	if (listen){
		/* Switch to continuous listen mode -  only for gateway */
		netdev->driver->set(netdev, NETOPT_LORA_SINGLE_RECEIVE, false, sizeof(uint8_t));
		sx127x_set_rx(&sx127x);
		printf("Listen mode set\n");
	}
	
}

int lora_send_frame(lora_frame payload)
{
	char txframe[9] = "";
	txframe[0] = payload.nodeID;
	txframe[1] = payload.frameID;
	strncat(txframe, payload.data,6);
    printf("sending Node ID = \"%d\"  Command = \"%d\"  Data = \"%s\" (%d bytes)\n",
          payload.nodeID , payload.frameID, payload.data ,strlen(txframe)+1);
		   
	struct iovec vec[1];
    vec[0].iov_base = txframe;
    vec[0].iov_len = strlen(txframe)+1;
    netdev->driver->send(netdev, vec, 1);

    /* wait for the chip */
    xtimer_usleep(10000);

    return 0;
}

static void _event_cb(netdev_t *dev, netdev_event_t event)
{
    if (event == NETDEV_EVENT_ISR) {
        msg_t msg;

        msg.type = MSG_TYPE_ISR;
        msg.content.ptr = dev;
		
        if (msg_send(&msg, _recv_pid) <= 0) {
            puts("gnrc_netdev: possibly lost interrupt.");
        }
    }
    else {
        size_t len;
        struct netdev_radio_rx_info rx_info;
		uint64_t tagId;	
        switch (event) {
            case NETDEV_EVENT_RX_COMPLETE:
                len = dev->driver->recv(dev, NULL, 5, &rx_info);
                dev->driver->recv(dev, message, len, NULL);
                // printf("{Payload: \"%s\" (%d bytes), RSSI: %i, SNR: %i}\n",
                       // message, (int)len, rx_info.rssi, (int) rx_info.snr);
					   
					   /* Handling incoming Messages */
					   switch(message[1])
					   {
							case FRAMEID_HEARTBEAT : //Heartbeat
								if(message[0] <= NODE_COUNT)
								{
									heartbeat_count[message[0]-1]++;
									printf(":NodeID:%d:heartbeat:%d:\n", message[0], heartbeat_count[message[0]-1]);
								} else {
									printf("Invalid  node ID received\n");
								}
								break;
							case FRAMEID_RFID_TAGID :
								memcpy(&tagId, &message[2], 5);
								tagId &= 0x3FFFFFFFFF;
								printf(":NodeID:%d:TagID:0x%x%x:\n" , message[0], (unsigned int)(tagId>>32),(unsigned int)tagId);					
								break;
                            case FRAMEID_FENCE_TRANSFERVAL :
                                printf(":NodeID:%d:FenceTransferVal:%d:\n", message[0],message[2]);
                            break;
							default:
								printf("Message without valid command received\n");
								break;
					   }
					   
                break;
            case NETDEV_EVENT_TX_COMPLETE:
                puts("Transmission completed");
                break;
            case NETDEV_EVENT_CAD_DONE:
                break;
            case NETDEV_EVENT_TX_TIMEOUT:
                break;
            default:
                printf("Unexpected netdev event received: %d\n", event);
                break;
        }
    }
}

void *_recv_thread(void *arg)
{
    static msg_t _msg_q[SX127X_LORA_MSG_QUEUE];
    msg_init_queue(_msg_q, SX127X_LORA_MSG_QUEUE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_ISR) {
            netdev_t *dev = msg.content.ptr;
            dev->driver->isr(dev);
        }
        else {
            puts("Unexpected msg type");
        }
    }
}