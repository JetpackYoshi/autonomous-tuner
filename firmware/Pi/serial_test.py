import serial
import time

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=9600,
    timeout=1)

ser.write(b'hello')
print(ser.readline())
