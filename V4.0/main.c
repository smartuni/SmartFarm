//#define ADC_NUMOF 16 /*Sonst Error*/

#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <math.h>

#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

/* Voltage_Control */
#include "thread_flags.h"
#include "periph/gpio.h"
#include "periph/adc.h"
/*RFID*/
#include "mlx90109.h"
#include "mlx90109_params.h"
#include "checksum/ucrc16.h"


#define RES             ADC_RES_16BIT
#define SIZE			3

int sleepTime = 2.2;
int sleepTime_VC = 5;
uint32_t handshake_rfid[2] = {0};
kernel_pid_t pid[4] ={0,0,0,0}; //main, RFID,Voltage Control,send ProzessID's speichern

/*Speicher reservieren*/
char voltage_control_thread_stack[THREAD_STACKSIZE_MAIN];
char rfid_thread_stack[THREAD_STACKSIZE_MAIN];
char send_thread_stack[THREAD_STACKSIZE_MAIN];

	/*RFID */
	mlx90109_t mlx90109_dev;
	tagdata neuerTag;
	
double messung (int anzahlMessungen, double umrechenfaktor)
{
	int sample;
	sample =0;//Test
	double spannung =0, peak=0,spannungMitteln[anzahlMessungen];
	xtimer_ticks32_t zeit2;
	zeit2=xtimer_now();
	xtimer_ticks32_t zeit3;
	zeit3=xtimer_now();
	while((zeit3.ticks32-zeit2.ticks32)<3000000)
	{
		sample = adc_sample(ADC_LINE(0), RES);
		spannung=(double)sample/umrechenfaktor;
		if(spannung>50){
			for(int i=0;i<anzahlMessungen;i++){
				sample = adc_sample(ADC_LINE(0), RES);
				spannungMitteln[i]=(double)sample/umrechenfaktor;
				
			}
			for(int i=0;i<anzahlMessungen;i++){
				peak=peak+spannungMitteln[i];
				//printf("Die Spannung auf ADC-Line 0 betraegt:%i\n",(int)spannungMitteln[i]);
			}
			peak=peak/anzahlMessungen;
			
			
			break;
		}
		zeit3=xtimer_now();
	}
	return peak;
}	

int berechnungUebergabewert(double spannungZaun, int torErkennung)
{
		int uebergabewert1 = 5;
		if(spannungZaun<200&&torErkennung==1){
			uebergabewert1=1; //Zaun kaputt / Tor geschlossen
		}
		else if(spannungZaun<200&&torErkennung==0){
			uebergabewert1=2; //Zaun kaputt / Tor geöffnet
		}
		else if(spannungZaun>=200&&torErkennung==1){
			uebergabewert1=3; //Zaun intakt / Tor geschlossen
		}
		else if(spannungZaun>=200&&torErkennung==0){
			uebergabewert1=4; //Zaun intakt / Tor geöffnet
		}
		else{
			uebergabewert1=5; //Fehler
		}
		
		return uebergabewert1;
}

int Torerkennung (int port, int pin)
{
		/*
		Die Funktion erkennt ob ein Tor offen oder geschlossen ist.
		Return 0 -> Tor ist offen
		Return 1 -> Tor ist geschlossen
		Return 2 -> Fehler beim initialisieren von Port und Pin
		*/
		if (gpio_init(GPIO_PIN(port, pin), GPIO_IN) < 0) //Port und Pin initialisieren
	  			{
      		  			printf("Error to initialize GPIO_PIN\n");
    					return 2; //Port und Pin wurden nicht initialisiert
    			}
		if (gpio_read(GPIO_PIN(port, pin))) 
				{
			printf("GPIO_PIN(%i.%02i) Tor ist offen\n", port, pin);//Pin -> high
			return 0;	
				}
		else {
        	printf("GPIO_PIN(%i.%02i) Tor ist geschlossen\n", port, pin);//Pin -> low
			return 1;
				}				
}




void *voltage_control_thread(void *arg)
{
	(void)arg;
	printf("Starte Spannungs_Kontrolle_thread, pid: %" PRIkernel_pid "\n", thread_getpid());
    msg_t m;
	
	if (adc_init(ADC_LINE(0)) < 0) {

           printf("Initialization of ADC_LINE(%i) failed\n", 0);

         

       } else {

           printf("Successfully initialized ADC_LINE(%i)\n", 0);

       }
	
    while(1)
	{
		/*Math stuff*/
		double spannung=0;
		int torErkennung=0;
		int uebergabewert=0;
		xtimer_sleep(sleepTime_VC);//5s
		spannung=messung(SIZE,191.8);
		//printf("Voltage on ADC-Line 0: %i[V]\n",(int)spannung);
		torErkennung=Torerkennung(0,1);
		uebergabewert=berechnungUebergabewert(spannung,torErkennung);
		/* Message Stuff*/
		m.content.value=uebergabewert*10+((int)thread_getpid());/*connect state and pid*/
		if((msg_try_send(&m,pid[0]))>=1);/*send to main*/
			else printf("VC send unsucceded:( \n");
	}
    return NULL;
}
	
	

//INTERRUPT ZUM AUSLESEN	
void interrupt_hand(void *args)
{
	if (args){}
	int16_t temp = 0;
//	printf("Dies ist ein Interrupt\n");
	temp = mlx90109_read(&mlx90109_dev);
	if (temp == MLX90109_DATA_OK)
	{
		//printf("data ok\n");
		temp = mlx90109_format(&mlx90109_dev, &neuerTag);
		if (temp == MLX90109_OK){
			neuerTag.newTag = 1;
			//printf("TagId: 0x%x%x \n" , (unsigned int)(neuerTag.tagId>>32),(unsigned int)neuerTag.tagId);
			//printf("CountryCode: %d \n" , (int)neuerTag.countryCode);
			//printf("\n\n");
		}
	}
	return;
}

void *rfid_thread(void *arg)
{
    (void) arg;
	int16_t error = 0;
    printf("rfid thread started, pid: %" PRIkernel_pid "\n", thread_getpid());
    msg_t m;
	
	
	error = mlx90109_init(&mlx90109_dev, &mlx90109_params[0], interrupt_hand);
	printf ("error: %d \n", error);
	printf("ISR Konfiguriert\n");

    while (1) {
		if (neuerTag.newTag == 1){
			neuerTag.newTag = 0;
			handshake_rfid[0] = (uint32_t)(neuerTag.tagId>>32);/*data here, cause msg_t just handle uint32_t and id is about 64Bit*/
			handshake_rfid[1] = (uint32_t)(neuerTag.tagId);
			m.content.value=1*10+((int)thread_getpid());/*connect state and pid*/
			if((msg_try_send(&m,pid[0]))>=1);/*sende  an main*/
			else printf("RFID send unsucceded :( \n");
			
			neuerTag.tagId = 0;
		}
		xtimer_sleep(sleepTime);
    }

    return NULL;
}
/*
* This thread changes messages between other boards
* Hopefully we can do it with the lora modules over a far range about 15km
* backup solution is to communicate over standard func
*
*/
void *send_thread(void *arg)
{
    (void) arg;
	
	return NULL;
}



int main(void)
{
printf("\n\n\n\n");
printf("******************+*********************************************************\n");
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
printf("*            -Voltage_Control: Jan-Hendrik meyer, Florian Alpheis.         *\n");
printf("*            -Website & App: Phillip Kroll, Sebastian Schulz               *\n");
printf("*            -Func & mainprogram: Dominik Zuerner, Nassim Agrebi           *\n");
printf("*                                                                          *\n");
printf("*   Version:2.0 (Release Candidate 1)                                      *\n");
printf("*   Last modification:06.06.2017                                           *\n");
printf("*   Name: Smartfarm.c                                                      *\n");
printf("*                                                                          *\n");
printf("*   To-Do: -CoAP-Server on node (IEEE 802.15.4 )                           *\n");
printf("*          -add lora-send function in main program                         *\n");
printf("*          -Error-handling                                                 *\n");
printf("*                                                                          *\n");
printf("*   NOTICE: This program runs with boards now!!!!                          *\n");
printf("*                                                                          *\n");
printf("****************************************************************************\n");
printf("\n\n\n\n");
    printf("main/send thread started, pid: %" PRIkernel_pid "\n", thread_getpid());
	pid[0] = (int)thread_getpid();
    msg_t m;/*message Object to send between threads*/
	/*Start Threads*/
    pid[1] = thread_create(voltage_control_thread_stack, sizeof(voltage_control_thread_stack),
                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                            voltage_control_thread, NULL, "voltage_control");
							
	pid[2] = thread_create(rfid_thread_stack, sizeof(rfid_thread_stack),
                            THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                            rfid_thread, NULL, "rfid");
							
	pid[3] = thread_create(send_thread_stack, sizeof(send_thread_stack),
                            THREAD_PRIORITY_MAIN - 3, THREAD_CREATE_STACKTEST,
                            send_thread, NULL, "send");


	/*handling msg from incoming and sending*/
    while (1) {
		m.content.value = 0;/*clear msg*/
		msg_receive(&m); /*1. receive and save message */
		/*int length,mask=0; 
		for(int exp =1;((unsigned int)(exp))<((unsigned int)m.content.value);length++)
		{ exp=exp*10; mask+=exp;}// maske erstellen*/
		int pid_msg = (m.content.value%10); /*2. filter pid */
		int state_msg = (m.content.value/10); /*3. filter data */
		/*4. handle data*/
		if(pid_msg>=1){
			/*handle voltage control msg*/
			if (pid_msg == (int)pid[1]){
				printf("Receive msg from VC, state:");
				switch(state_msg){
				case 1: printf("[Zaun kaputt,Tor geschlossen]\n");break;
				case 2: printf("[Zaun kaputt,Tor geoeffnet]\n");break;
				case 3: printf("[Zaun intakt,Tor geschlossen]\n");break;
				case 4: printf("[Zaun intakt,Tor geoeffnet]\n");break;
				case 5: printf("[Fehler mit dem Zaun]\n");break;
				}
				
			}
			/* handle RFID msg*/
			if (pid_msg == (int)pid[2]){
				printf("Receive msg from RFID, state:");
				switch(state_msg){
				case 1: printf("[new moooow] ID:%08x%08x\n",(unsigned int)handshake_rfid[0],(unsigned int)handshake_rfid[1]);break;
				case 2: printf("[BSE at cow] ID:%08x%08x\n",(unsigned int)handshake_rfid[0],(unsigned int)handshake_rfid[1]);break;
				case 3: printf("[Cow gonna go wild] ID:%08x%08x\n",(unsigned int)handshake_rfid[0],(unsigned int)handshake_rfid[1]);break;
				case 4: printf("[To many animals]\n");break;
				case 5: printf("[failed detected cow]\n");break;
				handshake_rfid[0] = 0; handshake_rfid[1] = 0;
				}
			}
		}
    }
}
