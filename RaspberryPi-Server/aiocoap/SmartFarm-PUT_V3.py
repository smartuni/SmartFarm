#!/usr/bin/env python3

import logging
import asyncio
import sys
import serial
import glob

from aiocoap import *

logging.basicConfig(level=logging.INFO)

def serial_ports():

        ports = glob.glob('/dev/tty[A-Za-z]*')
        result = []
        for port in ports:
            try:
                s=serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError,serial.SerialException):
                pass
        return result


def exclusiv_ports():
        List1 = serial_ports()
        print("Connect the LORA-Board to the Raspberry-Py")
        y = 0
        while (y != 1):
                x = input("Is the LORA-board connected ? y/n ")
                if x=="y":
                      List2 = serial_ports()
                      result = list(set(List1) ^ set(List2))
                      print(result)
                      y = 1
                elif x=="n":
                        print("Please connect the board")
                else:
                        print("Error")
                                       
        return result

def ports():
        y = 0
        while (y != 1):
                x = input("Is the LORA-board connected ? y/n ")
                if x=="y":
                        print("Please disconnect the board")
                elif x=="n":
                        port = exclusiv_ports()
                        
                        if not port:
                                print("ERROR: No Board found")
                        elif len(port)>1:
                                print("ERROR: To many Boards found")
                        elif len(port)==1:
                                serial = port[0]
                                serial = serial.strip()
                                print("Port: ", serial)
                                y = 1
                        else:
                                print("ERROR: WTF ")
                else:
                       print("Error")
        return serial

def start():
        y=0
        x=0
        print("Starting the Server")
        while (y != 1):
                x = input("Enabel 1: 6LoWPAN, 2: LoRaWAN, 3: Both ")
                if x=='1':
                        print("Starting 6LoWPAN")
                        y = 1
                if x=='2':
                        print("Starting LoRaWAN")
                        y = 1
                if x=='3':
                        print("Starting LoRaWAN and 6LoWPAN")
                        y = 1
                else:
                        print("Error: Try again")

        return x

async def LoWPAN(protocol,adress):
        #protocol = await Context.create_client_context()
        #Fence--------------
        path ="coap://[fe80::81d0:6d5e:52a5:432a%lowpan0]" + adress
        request = Message(code=GET, uri=path)

        try:
                response = await protocol.request(request).response
        except Exception as e:
                wert("Error:"+e)
                #print(e)
        else:
                print('Result: %s\n%r'%(response.code, response.payload))
                wert="getFence/" + str(response.payload)
                wert=wert.strip
                              
        List1 = wert.split(":")

        return List1
        

def LoRaWAN(ser):
        wert = str(ser.readline())
        #wert = "b'':NodeID:3:TagID:0x54F28CA9372B"
                              
        if (wert!="b''"):
                List1 = wert.split(":") # Split the string: ID:RFID:"ID"
                        #NoteID:--
                        #TagID:0x...
                        #First = ident.split("'")
                              
                if List1[1]=="NodeID":
                        load2 = "getLivestock/" + str(List1[4])

        else:
                print("/rwaiting..")

        #print('Result: %s\n%r'%(response.code, response.payload))
                              
        return load2

async def Server(context,load,payload):
        request = Message(code=PUT, payload=payload)
        request.opt.uri_host = '127.0.0.1'
        request.opt.uri_path = (load, "block")
        response = await context.request(request).response
       




async def main():

        context = await Context.create_client_context()
        await asyncio.sleep(2)

        network = start()
        print("return1")
        if (network=='2' or network=='3'):
                #port = "/dev/ttyACM0"
                print("searching Port..")
                port = ports()
                ser = serial.Serial(port, 115200, timeout=1) # ls /dev         

        speicher=""
                              
        while True:
                if (network=='1'): #6LoWPAN
                        FenceID = LoWPAN(context,"/fence/info" )
                        if(FenceID[0]=="Error"):
                              print(FenceID[1])
                        elif(FenceID[0]=="b'Fence info"):
                              load=FenceID[1]
                        payload = b"LoWPAN-Fence" * 30
                        Server(context,load,payload)

                        # Noch nicht umgesetzt vom LoWPAN Team
                        #RFID = 6LoWPAN(context,"/fence/rfid" )
                        
                              
                if (network=='2'): #LoRaWAN
                        load = LoRaWAN(ser)
                        payload = b"LoRaWAN" * 30
                        Server(context, load, payload)

                        
                if (network=='3'): #6LoWPAN and LoRaWAN
                        FenceID = LoWPAN(context,"/fence/info" )
                        if(FenceID[0]=="Error"):
                              print(FenceID[1])
                        elif(FenceID[0]=="b'Fence info"):
                              load=FenceID[1]
                              
                        payload = b"LoWPAN-Fence" * 30
                        Server(context, load2, payload)
                    
                        load = LoRaWAN(ser)
                        payload = b"LoRaWAN" * 30
                        Server(context, load, payload)

                              
                              
                


        if (network==2 or network==3):
                ser.close()
        


if __name__ == "__main__":
        asyncio.get_event_loop().run_until_complete(main())
