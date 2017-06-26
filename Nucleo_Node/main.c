#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <math.h>
#include "thread.h"
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "board.h"
#include "periph/rtc.h"

/* Lora */ 
#include "lora.h"

/* Fence */
#include "fence.h"

/*RFID*/
#include "mlx90109.h"
#include "mlx90109_params.h"
#include "checksum/ucrc16.h"

/* RX/TX Mode */
#define NODE_MODE_TX


int sleepTime = 2.2;
int sleepTime_VC = 5;
kernel_pid_t pid[4] ={0,0,0,0}; //main, RFID,fance,send ProzessID's speichern

/*allocated memory*/
 char fence_thread_stack[THREAD_STACKSIZE_MAIN];
 char lora_heartbeat_stack[THREAD_STACKSIZE_MAIN];
 char rfid_thread_stack[THREAD_STACKSIZE_MAIN];
 
/*RFID */
mlx90109_t mlx90109_dev;
tagdata neuerTag;  

void *fence_thread(void *arg)
{
	(void)arg;
	printf("start fence_thread, pid: %" PRIkernel_pid "\n", thread_getpid());
		/*Math stuff*/
		double voltage=0;
		int recogniseGate=0;
		int transferValue=0;
		char str[15];
			
		while(1){
				voltage=measure(SIZE,3);//3 for Nucleoboard
				printf("Voltage on ADC-Line 0: %i[V]\n",(int)voltage);
                recogniseGate=recognise_gate_funk(1,8);//PA3 = Gate
				transferValue=calculate_transfer_value(voltage,recogniseGate);
    	    	
    			sprintf(str, "Fence info: %i", transferValue);

				puts(str); 
		
                lora_frame payload;
		        payload.nodeID = 0x03;
	        	payload.frameID = FRAMEID_FENCE_TRANSFERVAL;
                payload.data[0] = (char) transferValue;
	        	lora_send_frame(payload);

		        xtimer_sleep(5); 
		  	
		}
}

//INTERRUPT ZUM AUSLESEN    
void interrupt_hand(void *args)
{
    if (args){}
    int16_t temp = 0;
    //printf("Dies ist ein Interrupt\n");
    temp = mlx90109_read(&mlx90109_dev);
    if (temp == MLX90109_DATA_OK)
    {
        //printf("data ok\n");
        temp = mlx90109_format(&mlx90109_dev, &neuerTag);
        if (temp == MLX90109_OK){
            neuerTag.newTag = 1;
			printf("TagId: 0x%x%x \n" , (unsigned int)(neuerTag.tagId>>32),(unsigned int)neuerTag.tagId);
        }
    }
}

void *rfid_thread(void *arg)
{
	(void) arg;
    int16_t error = 0;
    printf("rfid thread started, pid: %" PRIkernel_pid "\n", thread_getpid());
    error = mlx90109_init(&mlx90109_dev, &mlx90109_params[0], interrupt_hand);
    printf ("error: %d \n", error);
    printf("ISR Konfiguriert\n");
    while (1) {
        if (neuerTag.newTag == 1){
            neuerTag.newTag = 0;   

            /* Test for RFID and LORA*/
			lora_frame payload;
			payload.nodeID = 0x03;
			payload.frameID = FRAMEID_RFID_TAGID;	
			memcpy(&payload.data, &neuerTag.tagId, 5);
			lora_send_frame(payload);
            neuerTag.tagId = 0;
            xtimer_sleep(5);
        }
        xtimer_sleep(sleepTime);
    }
}

void *heartbeat_thread(void *arg)
{
	(void)arg;
	while(1)
    {
        printf("sending Heartbeat\n");
		lora_frame payload;
		payload.nodeID = 0x03;
		payload.frameID = FRAMEID_HEARTBEAT;
		lora_send_frame(payload);
		xtimer_sleep(10);	
	}
}

int main(void)
{
printf("\n\n\n\n");
printf("****************************************************************************\n");
printf("*                 Welcome To the Smart Farm Project                        *\n");
printf("*                                                                          *\n");
printf("*                      ZZZZZZ                                              *\n");
printf("*                    ZZZZZZZZZZZZ                                          *\n");
printf("*                  ZZZZZZZZZZZZZZZZ                                        *\n");
printf("*                 ZZZZZZZ     ZZZZZZ                                       *\n");
printf("*                ZZZZZZ        ZZZZZ                                       *\n");
printf("*                ZZZZZ          ZZZZ                                       *\n");
printf("*                ZZZZ           ZZZZZ                                      *\n");
printf("*                ZZZZ           ZZZZ                                       *\n");
printf("*                ZZZZ          ZZZZZ                                       *\n");
printf("*                ZZZZ        ZZZZZZ                                        *\n");
printf("*                ZZZZ     ZZZZZZZZ       777        7777       7777777777  *\n");
printf("*          ZZ    ZZZZ   ZZZZZZZZ         777      77777777    77777777777  *\n");
printf("*      ZZZZZZZ   ZZZZ  ZZZZZZZ           777     7777  7777       777      *\n");
printf("*    ZZZZZZZZZ   ZZZZ    Z               777     777    777       777      *\n");
printf("*   ZZZZZZ       ZZZZ                    777     777    777       777      *\n");
printf("*  ZZZZZ         ZZZZ                    777     777    777       777      *\n");
printf("* ZZZZZ          ZZZZZ    ZZZZ           777     777    777       777      *\n");
printf("* ZZZZ           ZZZZZ    ZZZZZ          777     777    777       777      *\n");
printf("* ZZZZ           ZZZZZ     ZZZZZ         777     777    777       777      *\n");
printf("* ZZZZ           ZZZZ       ZZZZZ        777     777    777       777      *\n");
printf("* ZZZZZ         ZZZZZ        ZZZZZ       777     777    777       777      *\n"); 
printf("*  ZZZZZZ     ZZZZZZ          ZZZZZ      777     7777777777       777      *\n");
printf("*   ZZZZZZZZZZZZZZZ            ZZZZ      777      77777777        777      *\n");
printf("*     ZZZZZZZZZZZ               Z                                          *\n");
printf("*        ZZZZZ                                                             *\n");
printf("*                                                                          *\n");
printf("*   OS: RIOT                                                               *\n");
printf("*   Kontext: Project at the HAW                                            *\n");
printf("*   Builder: -RFID: Jonas Radtke                                           *\n");
printf("*            -Fence: Jan-Hendrik meyer, Florian Alpheis.                   *\n");
printf("*            -Website & App: Phillip Kroll, Sebastian Schulz               *\n");
printf("*            -Func & mainprogram: Dominik Zuerner, Nassim Agrebi           *\n");
printf("*                                                                          *\n");
printf("*   Version:2.0 (Final Presentation)                                       *\n");
printf("*   Last modification:25.06.2017                                           *\n");
printf("*                                                                          *\n");
printf("*   To-Do: -CoAP-Server on node (IEEE 802.15.4 )                           *\n");
printf("*          -add lora-send function in main program                         *\n");
printf("*          -send and receive at same Time(LoRa)                            *\n");
printf("*          -Error-handling                                                 *\n");
printf("*                                                                          *\n");
printf("*   NOTICE: This program runs with boards now!!!!                          *\n");
printf("*                                                                          *\n");
printf("****************************************************************************\n");
printf("\n\n\n\n");

    printf("main/send thread started, pid: %" PRIkernel_pid "\n", thread_getpid());
    pid[0] = (int)thread_getpid();

    #if defined(NODE_MODE_TX)
    /* initalize lora 0 = TX mode 1 = RX mode */
    lora_init(0);
    /*Start Threads*/
    pid[1] = thread_create(fence_thread_stack, sizeof(fence_thread_stack),
                            THREAD_PRIORITY_MAIN - 3, THREAD_CREATE_STACKTEST,
                            fence_thread, NULL, "fence");                      

    pid[2] = thread_create(rfid_thread_stack, sizeof(rfid_thread_stack),
                            THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                            rfid_thread, NULL, "rfid");

    pid[3] = thread_create(lora_heartbeat_stack, sizeof(lora_heartbeat_stack),
                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                            heartbeat_thread, NULL, "thread");  
    #endif
    #if defined(NODE_MODE_RX)
    /* initalize lora 0 = TX mode 1 = RX mode */
    lora_init(1);
    #endif
    
							
} // end main

