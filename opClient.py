import websocket
import json

S0 = 421.53        
K = 460.00         
T = 0.25        
r = 0.051       
sigma = 0.185   
N = 50        
M = 100

conn = websocket.create_connection('ws://localhost:8080')

optionPrice = 5.00

vol0 = 0.1
vol1 = 0.9

while True:
    respA = conn.send(json.dumps([S0, K, r, vol0, T, N, M]))
    respA = json.loads(conn.recv())

    respB = conn.send(json.dumps([S0, K, r, vol0 - 0.01, T, N, M]))
    respB = json.loads(conn.recv())

    respC = conn.send(json.dumps([S0, K, r, vol0 + 0.01, T, N, M]))
    respC = json.loads(conn.recv())

    vega = (respC['price'] - respB['price'])/(2.0*0.01)
    price = respA['price']

    vol1 = vol0 - (price - optionPrice)/vega

    if abs(vol1 - vol0) < 0.00001:
        break

    vol0 = vol1

print("Implied Volatility: ", vol1)

respPrice = conn.send(json.dumps([S0, K, r, vol1, T, N, M]))
respPrice = json.loads(conn.recv())

print("Checked Price: ", respPrice['price'])