#!/usr/bin/env python3

import logging
import asyncio
import sys
import serial
import glob

from aiocoap import *

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
                x = input("Is the LORA-board connected ? y/n")
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
                x = input("Is the LORA-board connected ? y/n")
                if x=="y":
                        print("Please disconnect the board")
                elif x=="n":
                        port = exclusiv_ports()
                        
                        if not port:
                                print("ERROR: No Board found")
                        elif len(port)>1:
                                print("ERROR: To many Boards found")
                        elif len(port)==1:
                                print("Port: ", port)
                                serial = port[0]
                                print(port[0])
                                serial = serial.strip()
                                print(serial)
                                y = 1
                        else:
                                print("ERROR: WTF ")
                else:
                       print("Error")
        return serial


       
    
#ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1) # ls /dev

logging.basicConfig(level=logging.INFO)


async def main():

        #port = "/dev/ttyACM0"
        port = ports()
        ser = serial.Serial(port, 115200, timeout=1) # ls /dev
        
        
                #print(serial_ports())
                #serial = input("Serial-Port eingeben: ")

    
        context = await Context.create_client_context()

        await asyncio.sleep(2)
        speicher=""
        while True:
                wert = str(ser.readline())
                #wert = "b'':NodeID:3:TagID:0x54F28CA9372B"
                print(wert)
                if (wert!="b''"):
                        #payload = wert.encode()
                        List1 = wert.split(":") # Split the string: ID:RFID:"ID"

                        #NoteID:--
                        #TagID:0x...
                        #First = ident.split("'")
                        if List1[1]=="NodeID":
                                #if List1[4]!=speichen:
                                
                                
                                print(List1)
                                #print(List1[1])
                                print(List1[4])
                                #load = int(List1[4],0)
                                payload = b"Livestock" * 30
                                #load2 = "getLivestock/" + str(load)
                                load2 = "getLivestock/" + str(List1[4])
                                #load = str(int(List1[4],0))
                                #print(load)
                                print(load2)
                                request = Message(code=PUT, payload=payload)
                                request.opt.uri_host = '127.0.0.1'
                                request.opt.uri_path = (load2, "block")
                                #request.opt.uri_path = ("getusers", "block")
                                #request.opt.uri_path = ("getLivestock/",93400718763) # Blockargument für Stringübergabe
                                #request.opt.uri_path = ("getLivestock/",load) # Blockargument für Stringübergabe
                                #request.opt.uri_path = ("getLivestock/",load, "block") # Blockargument für Stringübergabe
                                response = await context.request(request).response

                                #print('Result: %s\n%r'%(response.code, response.payload))
                #else:
                 #       print("waiting..", end='/r')

        ser.close()
        


if __name__ == "__main__":
        asyncio.get_event_loop().run_until_complete(main())
