import asyncio
import websockets
import json
import ctypes

option = ctypes.CDLL("./computer.so")

option.OptionPrice.argtypes = (
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_int,
    ctypes.c_int
)

option.OptionPrice.restype = ctypes.c_double


async def Server(ws, path):
    while True:
        resp = await ws.recv()
        resp = json.loads(resp)
        S, K, r, iv, t, N, M = resp
        optionPrice = option.OptionPrice(S, K, r, iv, t, N, M)
        message = {'price': optionPrice}
        await ws.send(json.dumps(message))

print("Server Booted")
loop = asyncio.get_event_loop()
loop.run_until_complete(websockets.serve(Server, 'localhost', 8080))
loop.run_forever()