# hw4
## Pinout
* L Servo: D12
* R Servo: D13
* mbed Xbee:<br>
  RX -> D0<br>
  TX -> D1
* OpenMV:<br>
  RX(P5) -> A0<br>
  TX(P4) -> A1

## For pyserial
```console
sudo apt install python3-dev<br>
sudo apt install python3-pip<br>
sudo -H pip3 install pyserial
```

## For PC Xbee Setting
In the demo directory, run
```console
sudo python3 Xbee_setting.py
```

## Include the BB Car Control Library
```console
mbed add https://gitlab.larc-nthu.net/ee2405_2021/bbcar
```

---
## Demo Videos
https://drive.google.com/drive/folders/1TCFKUdYpighEKNE049Uwt63aagqrl9gP?usp=sharing

## XBee Controlled BB Car
* demo folder: demo_1
* case 1<br>
d1 = 10 cm<br>
d2 = 10 cm<br>
```console
sudo python3 hw4_1_1.py
```

* case 2<br>
d1 = 20 cm<br>
d2 = 5 cm<br>
```console
sudo python3 hw4_1_2.py
```

## Line Following BB Car
* demo folder: demo_2
* OpenMV<br>
  main.py附於OpenMV的資料夾中
* 因為相機不是朝向正下方，直線的方向在其視角下會較複雜，所以我只利用直線的一個端點完成這題。

## BB Car Position Calibration
