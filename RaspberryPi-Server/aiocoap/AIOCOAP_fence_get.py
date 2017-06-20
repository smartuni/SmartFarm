
import logging
import asyncio

from aiocoap import *

logging.basicConfig(level=logging.INFO)

async def main():
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://[fe80::81d0:6d5e:52a5:432a%lowpan0]/fence/info')

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))



if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())

