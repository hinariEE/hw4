#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "myXbee.h"

Ticker servo_ticker;
PwmOut servoL(D12), servoR(D13);
BBCar car(servoL, servoR, servo_ticker);
BufferedSerial pc(USBTX, USBRX);

void rpc_goStraightCalib(Arguments *in, Reply *out){
    double speed = in->getArg<double>();
    car.goStraightCalib(speed);
    return;
}

void rpc_spinLeft(Arguments *in, Reply *out){
    double speed = in->getArg<double>();
    double factor = in->getArg<double>();
    car.spinCalib(speed, factor);
    ThisThread::sleep_for(2s);
    car.stop();
    return;
}

RPCFunction Myrpc1(&rpc_goStraightCalib, "goStraightCalib");
RPCFunction Myrpc2(&rpc_spinLeft, "spinLeft");



int main() {
    double pwm_table0[] = {-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
    double speed_table0[] = {-16.264, -15.947, -15.947, -15.947, -15.628, -14.990, -14.352, -13.395, -12.438, -10.525, -8.611, -6.379, -4.146, -1.834, -0.000, 0.558, 2.551, 4.943, 7.017, 9.249, 10.844, 12.438, 13.714, 14.352, 14.990, 15.628, 15.947, 15.947, 16.266, 16.266, 16.266};
    double speed_table1[] = {-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.870, -0.717, 0.000, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584};
    car.setCalibTable(31, pwm_table0, speed_table0, 31, pwm_table0, speed_table1);

    car.stop();
    xbee_init();

    char buf[256] = {0}, outbuf[256] = {0};
    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    /*while(1) {
        for(int i = 0; ; i++) {
            char recv = fgetc(devin);
            if(recv == '\n') {
                buf[i] = 0;
                printf("\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
        fprintf(devout, "%s\n", outbuf);
    }*/

}