import serial
import time
serdev = '/dev/ttyACM0'                # use the device name you get from `ls /dev/ttyACM*`
s = serial.Serial(serdev, 9600)

s.write("/spinLeft/run 5 -1\n".encode())
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
s.close()
