# hw4
## Pinout
* L Servo: D12
* R Servo: D13
* mbed Xbee:<br>
  RX -> D0<br>
  TX -> D1

## For pyserial
```console
sudo apt install python3-dev<br>
sudo apt install python3-pip<br>
sudo -H pip3 install pyserial
```

## For PC Xbee Setting
```console
sudo python3 Xbee_setting.py
```

## Include the BB Car Control Library
```console
mbed add https://gitlab.larc-nthu.net/ee2405_2021/bbcar
```
---
## XBee Controlled BB Car
* demo video: in Teams
* case 1.<br>
d1 = 10 cm<br>
d2 = 10 cm<br>
run hw4_1_1.py
* case 2.<br>
d1 = 20 cm<br>
d2 = 5 cm<br>
run hw4_1_2.py

## Line Following BB Car
## BB Car Position Calibration
