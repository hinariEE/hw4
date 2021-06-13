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
}

void rpc_spinLeft(Arguments *in, Reply *out){
    double speed = in->getArg<double>();
    double factor = in->getArg<double>();
    car.spinCalib(speed, factor);
    ThisThread::sleep_for(2s);
    car.stop();
}

DigitalIn encoder(D10);
Ticker encoder_ticker;
volatile int steps;
volatile int last;
void encoder_control(){
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

using namespace std::chrono;
void rpc_goBySec(Arguments *in, Reply *out){
    double sec = in->getArg<double>();
    steps = 0;
    car.goStraightCalib(10);
    ThisThread::sleep_for(duration_cast<milliseconds>(duration<double>(sec)));
    car.stop();
    ThisThread::sleep_for(2500ms);
    char buf[16];
    sprintf(buf, "%7.3f", (float)steps * 6.5 * 3.14 / 32);
    out->putData(buf);
}

double cmToSec(double cm){
    if(cm < 0)
        cm = -cm;
    return cm / 9.9 + 0.3;
}

void goByCm(double cm){
    double sec = cmToSec(cm);
    if(cm >= 0)
        car.goStraightCalib(10);
    else
        car.goStraightCalib(-10);

    ThisThread::sleep_for(duration_cast<milliseconds>(duration<double>(sec)));
    car.stop();
}

void rpc_goByCm(Arguments *in, Reply *out){
    double cm = in->getArg<double>();
    goByCm(cm);
}

void rpc_rPark(Arguments *in, Reply *out){
    double d1 = in->getArg<double>();              // edge-wheel: 8 cm
    double d2 = in->getArg<double>() + 8.0 + 2.0;  // slot width: 4 cm wider than the car
    double goDist = d1 - d2;
    goByCm(goDist);
    ThisThread::sleep_for(1500ms);
    double R = d1 + 11.0;                          // turning radius: R; car width: 11 cm
    double factor = 1.0 - 11.0 / R;                // R = 11 / (1 - factor) (cm);
    if(factor == 0.0)
        factor = 1e-6;
    double path = 6.28 * R / 4.0; // 3.14 * 2 => 6.0
    double sec = cmToSec(path);
    car.turnCalib(-10, -factor);
    ThisThread::sleep_for(duration_cast<milliseconds>(duration<double>(sec)));
    car.stop();
    ThisThread::sleep_for(1s);
    goByCm(-5);
}

RPCFunction Myrpc1(&rpc_goStraightCalib, "goStraightCalib");
RPCFunction Myrpc2(&rpc_spinLeft, "spinLeft");
RPCFunction Myrpc3(&rpc_goBySec, "goBySec");
RPCFunction Myrpc4(&rpc_goByCm, "goByCm");
RPCFunction Myrpc5(&rpc_rPark, "rPark");

int main() {
    encoder_ticker.attach(&encoder_control, 10ms);

    double pwm_table0[] = {-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
    double speed_table0[] = {-16.264, -15.947, -15.947, -15.947, -15.628, -14.990, -14.352, -13.395, -12.438, -10.525, -8.611, -6.379, -4.146, -1.834, -0.000, 0.558, 2.551, 4.943, 7.017, 9.249, 10.844, 12.438, 13.714, 14.352, 14.990, 15.628, 15.947, 15.947, 16.266, 16.266, 16.266};
    double speed_table1[] = {-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.870, -0.717, 0.000, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584};
    car.setCalibTable(31, pwm_table0, speed_table0, 31, pwm_table0, speed_table1);

    car.stop();
    xbee_init();

    char buf[256] = {0}, outbuf[256] = {0};
    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    while(1) {
        for(int i = 0; ; i++) {
            char recv = fgetc(devin);
            if(recv == '\n') {
                buf[i] = '\0';
                printf("\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
        fprintf(devout, "%s\n", outbuf);
    }

}