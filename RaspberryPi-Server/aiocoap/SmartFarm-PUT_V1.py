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
        
    
#ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1) # ls /dev

logging.basicConfig(level=logging.INFO)


async def main():
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
                                serial = serial.strip()
                                y = 1
                                ser = serial.Serial(serial, 115200, timeout=1) # ls /dev
                        else:
                                print("ERROR: WTF ")
                else:
                       print("Error") 


                #print(serial_ports())
                #serial = input("Serial-Port eingeben: ")




    
        
    
        context = await Context.create_client_context()

        await asyncio.sleep(2)
        while True:
                wert = str(ser.readline())

              
                if (wert!="b''"):
                        print (wert)
                        wert2 = wert.encode()
                        #payload = b"payload Text" * 30
                        payload = wert2
                        request = Message(code=PUT, payload=payload)
                        request.opt.uri_host = '127.0.0.1'
                        request.opt.uri_path = ("getusers", "block") # Blockargument für Stringübergabe

                        response = await context.request(request).response

                        print('Result: %s\n%r'%(response.code, response.payload))
                    
                ser.close()
        


if __name__ == "__main__":
        asyncio.get_event_loop().run_until_complete(main())
