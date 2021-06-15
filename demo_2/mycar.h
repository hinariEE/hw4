#ifndef MYCAR_H
#define MYCAR_H

#include "bbcar.h"
#include "mbed_rpc.h"

class mycar : public BBCar {
public:
    mycar( PwmOut &pin_servo0, PwmOut &pin_servo1, Ticker &servo_ticker );

    void stopCalib();

    void turnCalib( double speed, double factor );
    
    // turn left/right in place
    void spinCalib( double speed );
    
    void goByCm(double cm);
    
    void rPark(double d1, double d2);
};

extern mycar car;

double cmToSec(double cm);
void rpc_goStraightCalib(Arguments *in, Reply *out);
void rpc_spin_2s(Arguments *in, Reply *out);
void rpc_goByCm(Arguments *in, Reply *out);
void rpc_rPark(Arguments *in, Reply *out);
#endif