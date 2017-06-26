[previous Version](../V3.0)

# SmartFarm Project

## Overiew
RFID-Part
- send RFID of Tag
- send status of checksum
    
Voltage-Control
- send if fence is broken or if sth. lies on it
- send if animal-gate is open or closed

LoRA
- send and receive data
	
IEEE 802.15.4
- send upd pakages between client and server
	
Server
- receive data from LoRa-Module via UART
- send data to CoAP-Server

To flash tap
```sh
$ BOARD=nucleo-f411 make all flash term
```
## Used Boards
- Selfmade RFID-Board
- Nucleo-F441
- mbed SX1276MB1xAS LoRa Shield


## Send packages via LoRa
Necessary lib:
```C
#include "lora.h"
```

The nodeID and the frameID enums are filled with
```C
typedef enum lora_nodeID
{
    NODEID_NODEone 				= 0x01,
	NODEID_NODEtwo				= 0x02
} lora_nodeID;

typedef enum lora_frameID
{
    FRAMEID_HEARTBEAT 			= 0x01,
	FRAMEID_RFID_TAGID			= 0x02,
	FRAMEID_FENCE				= 0x10
	
} lora_frameID;
```
feel free to extend them




Make a Frame
```C
lora_frame frame;
```
The Frame has the form
```C
typedef struct lora_frame {
    uint8_t	nodeID;
	uint8_t frameID;
    char data[6];
} lora_frame ;
```
So let's fill the struct with a NodeID
```C
frame.nodeID = 0x03;
```
and with a FrameID
```C
frame.frameID = FRAMEID_RFID_TAGID;
```

Fill the struct with some payload:
```C
char payload[6] = {0}; 
            .
            .
            .
frame.data[0] = payload[0];
frame.data[1] = payload[1];
frame.data[2] = payload[2];
frame.data[3] = payload[3];
frame.data[4] = payload[4]; 
frame.data[5] = payload[5]; 
```
Now we send our frame via LoRa:
```C
lora_send_frame(frame);
```

## RFID
To begin convince that u include the librarys for the mlx90109
```C
#include "mlx90109.h"
#include "mlx90109_params.h"
#include "checksum/ucrc16.h"
```

Define Clock and Data Pin fit to the used hardware
```C
#define MLX90109_PARAM_CLOCK
#define MLX90109_PARAM_DATA  
```

Create device descriptor
```C
mlx90109_t mlx90109_dev;
```
The device descripter has the form
```C
typedef struct {
    mlx90109_params_t p; 		/**< device configuation parameter */
	uint8_t counter;	  		/**< counter for data bits*/
	uint8_t counter_header;		/**< counter for Header bits "10000000000"*/
	uint8_t data[128];			/**< raw data*/
} mlx90109_t;
```
Then we create a Tag Object
```C
tagdata newTag;  
```

Then make an interrupt for detecting new tags in the near field
```C
void interrupt_hand(void *args)
{
    if (args){}
    int16_t temp = mlx90109_read(&mlx90109_dev);
    if (temp == MLX90109_DATA_OK)
    {
        printf("data ok\n");
    }
return;
}
```

Use the format-function to make raw-data(just a bitstring) to tagData (formated in TagID and countrycode). In this function also a checksum test is running in the background.
```C
mlx90109_format(&mlx90109_dev, &neuerTag);
```
## Fence
Necessary lib:
```c
#include "fence.h"
```
Measure the voltage at fence
```c
double measure (int numberOfMeasurement, double calculateParameter)
```

detecting the Gate state: 
```c
recognise_gate_funk (int port, int pin);
```
|Value | Fence |
| ------ | ------ |
|0|open|
|1|closed|
|2|error at initialisation of the port|


calculate transfer value 
```c
calculate_transfer_value(double voltageFence, int recogniseGate)
```
|Value | Fence | Gate |
| ------ | ------ | ------|
|1|broke|closed|
|2|broke|open|
|3|intact|closed|
|4|intact|open|


	
## To-Do:
    - bidirectorial communication (LoRa)  
	- Testing under real conditions
    - Error-handling  
	



