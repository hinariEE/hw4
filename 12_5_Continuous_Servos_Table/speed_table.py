import matplotlib.pyplot as plt
import numpy as np
import serial

Ts = 10;   # signal interval
end = 150; # signal end point
n = int(end / Ts) * 2 + 1;

x = np.linspace(-end, end, num=n) # x axis
y = np.zeros(n)                   # y axis

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)

print('{', end='')
for i in range(0, n - 1):
    print('%d' % x[i], end=', ')
print('%d' % x[n - 1], end='')
print('}')

print('{', end='')
for i in range(0, (n - 1) // 2):
    line = s.readline() # Read a string from B_L4S5I_IOT01A terminated with '\n'
    y[i] = -float(line)
    print(y[i], end=', ')

for i in range((n - 1) // 2, n - 1):
    line = s.readline() # Read a string from B_L4S5I_IOT01A terminated with '\n'
    y[i] = float(line)
    print(y[i], end=', ')

line = s.readline() # Read a string from B_L4S5I_IOT01A terminated with '\n'
y[n - 1] = float(line)
print(y[n - 1], end='')
print('}')

plt.figure()
#x = [-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150]
#y = [-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.87, -0.717, 0.0, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584]
plt.plot(x,y)
plt.xlabel('signal')
plt.ylabel('speed (cm/sec)')
plt.show()
s.close()

#{-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150}
# L
#{-16.264, -15.947, -15.947, -15.947, -15.628, -14.990, -14.352, -13.395, -12.438, -10.525, -8.611, -6.379, -4.146, -1.834, -0.000, 0.558, 2.551, 4.943, 7.017, 9.249, 10.844, 12.438, 13.714, 14.352, 14.990, 15.628, 15.947, 15.947, 16.266, 16.266, 16.266}
# R
#{-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.870, -0.717, 0.000, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584}

