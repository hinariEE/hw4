import matplotlib.pyplot as plt
import serial
import numpy as np

# XBee setting
serdev = '/dev/ttyUSB0'                # '/dev/ttyACM0' or '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
'''
ms_list = []
cm_list = []
for ms in np.arange(0, 6.1, 0.1):
    s.write("/goBySec/run {:g}\n".format(ms).encode())
    line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
    #print(line)
    line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
    #print(line)
    cm = float(line.decode())
    print('{:7.3f}'.format(cm))
    ms_list.append(ms)
    cm_list.append(cm)
print('ms_list = [', end='')
for ms in ms_list[:-1]:
    print('{:.3f}'.format(ms), end=', ')
print('{:.3f}'.format(ms_list[-1]), end='')
print('];')
print('cm_list = [', end='')
for cm in cm_list[:-1]:
    print('{:.3f}'.format(cm), end=', ')
print('{:.3f}'.format(cm_list[-1]), end='')
print(']')
'''
s.close()

ms_list = [0.000, 0.100, 0.200, 0.300, 0.400, 0.500, 0.600, 0.700, 0.800, 0.900, 1.000, 1.100, 1.200, 1.300, 1.400, 1.500, 1.600, 1.700, 1.800, 1.900, 2.000, 2.100, 2.200, 2.300, 2.400, 2.600, 2.700, 2.800, 2.900, 3.000, 3.100, 3.200, 3.300, 3.400, 3.500, 3.600, 3.700, 3.800, 3.900, 4.000, 4.100, 4.200, 4.300, 4.400, 4.500, 4.600, 4.700, 4.800, 4.900, 5.000, 5.100, 5.200, 5.400, 5.500, 5.600, 5.700, 5.800, 5.900, 6.000]
cm_list = [0.000, 0.000, 0.638, 0.000, 1.276, 1.913, 3.189, 4.465, 5.740, 7.016, 8.292, 9.567, 10.205, 11.481, 12.118, 13.394, 14.032, 14.670, 16.583, 16.583, 17.859, 19.134, 19.772, 21.048, 21.686, 23.599, 24.875, 25.512, 26.788, 27.426, 28.702, 29.339, 30.615, 31.253, 32.528, 33.166, 34.442, 35.080, 36.355, 36.993, 38.269, 38.907, 40.182, 41.458, 42.096, 42.733, 44.009, 44.647, 45.922, 47.198, 47.836, 48.474, 50.387, 51.663, 52.938, 53.576, 54.852, 55.490, 56.765]
plt.plot(ms_list, cm_list)
plt.plot([0.3, 6], [0, (6 - 0.3) * 9.9], label=r'$x = 9.9 \bullet (t-0.3)$')
plt.legend()
plt.xlabel('time (sec)')
plt.ylabel('distance (cm)')
plt.show()

# ms_list = [0.000, 0.100, 0.200, 0.300, 0.400, 0.500, 0.600, 0.700, 0.800, 0.900, 1.000, 1.100, 1.200, 1.300, 1.400, 1.500, 1.600, 1.700, 1.800, 1.900, 2.000, 2.100, 2.200, 2.300, 2.400, 2.500, 2.600, 2.700, 2.800, 2.900, 3.000, 3.100, 3.200, 3.300, 3.400, 3.500, 3.600, 3.700, 3.800, 3.900, 4.000, 4.100, 4.200, 4.300, 4.400, 4.500, 4.600, 4.700, 4.800, 4.900, 5.000, 5.100, 5.200, 5.300, 5.400, 5.500, 5.600, 5.700, 5.800, 5.900, 6.000]
# cm_list = [0.000, 0.000, 0.638, 0.000, 1.276, 1.913, 3.189, 4.465, 5.740, 7.016, 8.292, 9.567, 10.205, 11.481, 12.118, 13.394, 14.032, 14.670, 16.583, 16.583, 17.859, 19.134, 19.772, 21.048, 21.686, 24.875, 23.599, 24.875, 25.512, 26.788, 27.426, 28.702, 29.339, 30.615, 31.253, 32.528, 33.166, 34.442, 35.080, 36.355, 36.993, 38.269, 38.907, 40.182, 41.458, 42.096, 42.733, 44.009, 44.647, 45.922, 47.198, 47.836, 48.474, 50.387, 50.387, 51.663, 52.938, 53.576, 54.852, 55.490, 56.765]
# modified
# ms_list = [0.000, 0.100, 0.200, 0.300, 0.400, 0.500, 0.600, 0.700, 0.800, 0.900, 1.000, 1.100, 1.200, 1.300, 1.400, 1.500, 1.600, 1.700, 1.800, 1.900, 2.000, 2.100, 2.200, 2.300, 2.400, 2.600, 2.700, 2.800, 2.900, 3.000, 3.100, 3.200, 3.300, 3.400, 3.500, 3.600, 3.700, 3.800, 3.900, 4.000, 4.100, 4.200, 4.300, 4.400, 4.500, 4.600, 4.700, 4.800, 4.900, 5.000, 5.100, 5.200, 5.400, 5.500, 5.600, 5.700, 5.800, 5.900, 6.000]
# cm_list = [0.000, 0.000, 0.638, 0.000, 1.276, 1.913, 3.189, 4.465, 5.740, 7.016, 8.292, 9.567, 10.205, 11.481, 12.118, 13.394, 14.032, 14.670, 16.583, 16.583, 17.859, 19.134, 19.772, 21.048, 21.686, 23.599, 24.875, 25.512, 26.788, 27.426, 28.702, 29.339, 30.615, 31.253, 32.528, 33.166, 34.442, 35.080, 36.355, 36.993, 38.269, 38.907, 40.182, 41.458, 42.096, 42.733, 44.009, 44.647, 45.922, 47.198, 47.836, 48.474, 50.387, 51.663, 52.938, 53.576, 54.852, 55.490, 56.765]