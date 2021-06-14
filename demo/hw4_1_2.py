import matplotlib.pyplot as plt
import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'                # '/dev/ttyACM0' or '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
s.write("/rPark/run 10 10\n".encode())
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
s.close()
