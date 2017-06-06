[previous Version](../V3.0)

# Main from SmartFarm Project

## Funktions
RFID-Part
    
    - send RFID of Tag
    - send status of checksum
    
Voltage_Control

     - send if fence is broken or if sth. lies on it
     - send if animal-gate is open or closed
LoRA
	-send and receive data
	
IEEE 802.15.4
	
	- send upd pakages between client and server
	

	
Server
	-receive data from LoRa-Module via UART
	-send data to CoAP-Server

    
## To-Do:
    -CoAP-Server on node / add code to main function (IEEE 802.15.4 )
	-add LoRA code to main function
    -send and receive at 'same' time (LoRa)       
    -Error-handling  
	



