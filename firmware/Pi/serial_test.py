import serial
import time

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=115200,
    timeout=1)

ser.write(b'2;')
print(ser.readline())
