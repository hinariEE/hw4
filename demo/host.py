import matplotlib.pyplot as plt
import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'                # '/dev/ttyACM0' or '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
s.write("/rPark/run 5 10\n".format(ms).encode())
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
ms_list.append(ms)
cm_list.append(float(line.decode()))
s.close()

# R = 11 / (1 - factor) (cm)