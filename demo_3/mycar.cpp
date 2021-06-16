#include "mycar.h"
using namespace std::chrono;

mycar::mycar( PwmOut &pin_servo0, PwmOut &pin_servo1, Ticker &servo_ticker ) : BBCar(pin_servo0, pin_servo1, servo_ticker){

}

void mycar::stopCalib(){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed_by_cm(0);
    servo1.set_speed_by_cm(0);
}

void mycar::turnCalib( double speed, double factor ){
    if(factor>0){
        servo0.set_factor(factor);
        servo1.set_factor(1);
    }
    else if(factor<0){
        servo0.set_factor(1);
        servo1.set_factor(-factor);
    }
    servo0.set_speed_by_cm(speed);
    servo1.set_speed_by_cm(-speed);
}

void mycar::spinCalib( double speed ){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed_by_cm(-speed);
    servo1.set_speed_by_cm(-speed);
}

void mycar::goByCm(double cm){
    double sec = cmToSec(cm);
    auto ms = duration_cast<milliseconds>(duration<double>(sec));
    if(cm >= 0)
        car.goStraightCalib(10);
    else
        car.goStraightCalib(-10);

    ThisThread::sleep_for(ms);
    stopCalib();
}

void mycar::rPark(double d1, double d2){
               // wheel-car spacing: 1.5 cm
    d2 += 8.0; // edge-wheel: 8 cm, slot width: 4 cm wider than the car
    double goDist = d1 - d2;
    goByCm(goDist);
    ThisThread::sleep_for(1500ms);
    double R = d1 + 11.0;           // turning radius: R; car width: 11 cm
    double factor = 1.0 - 11.0 / R; // R = 11 / (1 - factor) (cm);
    if(factor == 0.0)
        factor = 1e-6;
    double path = 6.28 * R / 4.0;   // 3.14 * 2 => 6.28, a quarter circle => 1 / 4
    double sec = cmToSec(path);
    auto ms = duration_cast<milliseconds>(duration<double>(sec));
    car.turnCalib(-10, -factor);
    ThisThread::sleep_for(ms);
    stopCalib();
    ThisThread::sleep_for(1500ms);
    goByCm(-5);
}

void mycar::spinDeg(double degree){
    double speed = 5.0;
    double path = 3.14 * 11.0 * degree / 360.0;
    double sec = abs(path) / speed + 0.3; // tuning based on trial-and-error
    if(path > 0)
        sec = sec * 18.0 / 17.0 - 0.119;
    auto ms = duration_cast<milliseconds>(duration<double>(sec));
    if(path >= 0){
        spinCalib(speed);
    }
    else{
        spinCalib(-speed);
    }
    ThisThread::sleep_for(ms);
    stopCalib();
}

double cmToSec(double cm){
    if(cm < 0)
        cm = -cm;
    return cm / 9.9 + 0.3;
}

RPCFunction rpcStopCalib(&rpc_stopCalib, "stopCalib");
RPCFunction rpcGoCalib(&rpc_goStraightCalib, "goStraightCalib");
RPCFunction myrpc2(&rpc_spin_2s, "spin_2s");
RPCFunction myrpc4(&rpc_goByCm, "goByCm");
RPCFunction rpcRPark(&rpc_rPark, "rPark");
RPCFunction rpcSpinDeg(&rpc_spinDeg, "spinDeg");

void rpc_stopCalib(Arguments *in, Reply *out){
    car.stopCalib();
}

void rpc_goStraightCalib(Arguments *in, Reply *out){
    double speed = in->getArg<double>();
    car.goStraightCalib(speed);
}

void rpc_spin_2s(Arguments *in, Reply *out){
    double speed = in->getArg<double>();
    car.spinCalib(speed);
    ThisThread::sleep_for(2s);
    car.stopCalib();
}

void rpc_goByCm(Arguments *in, Reply *out){
    double cm = in->getArg<double>();
    car.goByCm(cm);
}

void rpc_rPark(Arguments *in, Reply *out){
    double d1 = in->getArg<double>();
    double d2 = in->getArg<double>();
    car.rPark(d1, d2);
}

void rpc_spinDeg(Arguments *in, Reply *out){
    double degree = in->getArg<double>();
    car.spinDeg(degree);
}