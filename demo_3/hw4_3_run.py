import serial

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

s.write("/hw4_3/run\n".encode())

s.close()
