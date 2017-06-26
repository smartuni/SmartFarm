#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <math.h>

#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#include "fence.h"

//int sleepTime_VC = 5;

double measure (int numberOfMeasurement, double calculateParameter)
{
	adc_init(ADC_LINE(5));
	int sample;
	sample =0;//Test
	double voltage =0, peak=0,voltageAverage[numberOfMeasurement];
	xtimer_ticks32_t time2;
	time2=xtimer_now();
	xtimer_ticks32_t time3;
	time3=xtimer_now();

	while((time3.ticks32-time2.ticks32)<3000000)
	{
		sample = adc_sample(ADC_LINE(5), RES);
		voltage=(double)sample/calculateParameter;
		//printf("voltage raw:%d\n",sample);
		
		if(voltage>50){
			for(int i=0;i<numberOfMeasurement;i++){
				sample = adc_sample(ADC_LINE(5), RES);
				voltageAverage[i]=(double)sample/calculateParameter;
				
			}
			for(int i=0;i<numberOfMeasurement;i++){
				peak=peak+voltageAverage[i];
				//printf("Die Spannung auf ADC-Line 0 betraegt:%i\n",(int)voltageAverage[i]);
			}
			peak=peak/numberOfMeasurement;
			
			
			break;
		}
		time3=xtimer_now();
	}
	return peak;
}	
int calculate_transfer_value(double voltageFence, int recogniseGate)
{
		int transferValue1 = 5;
		
		if(voltageFence<200&&recogniseGate==1){
			transferValue1=1; //Zaun kaputt / Tor geschlossen
		}
		else if(voltageFence<200&&recogniseGate==0){
			transferValue1=2; //Zaun kaputt / Tor geöffnet
		}
		else if(voltageFence>=200&&recogniseGate==1){
			transferValue1=3; //Zaun intakt / Tor geschlossen
		}
		else if(voltageFence>=200&&recogniseGate==0){
			transferValue1=4; //Zaun intakt / Tor geöffnet
		}
		else{
			transferValue1=5; //Fehler
		}
	
		return transferValue1;
}
int recognise_gate_funk (int port, int pin)
{
		/*
		Die Funktion erkennt ob ein Tor offen oder geschlossen ist.
		Return 0 -> Tor ist offen
		Return 1 -> Tor ist geschlossen
		Return 2 -> Fehler beim initialisieren von Port und Pin
		*/
		if (gpio_init(GPIO_PIN(port, pin), GPIO_IN) < 0) //Port und Pin initialisieren
	  			{
      		  			//printf("Error to initialize GPIO_PIN\n");
    					return 2; //Port und Pin wurden nicht initialisiert
    			}
		if (gpio_read(GPIO_PIN(port, pin))) 
				{
			//printf("GPIO_PIN(%i.%02i) Tor ist offen\n", port, pin);//Pin -> high
			return 0;	
				}
		else {
        	//printf("GPIO_PIN(%i.%02i) Tor ist geschlossen\n", port, pin);//Pin -> low
			return 1;
				}				
}
