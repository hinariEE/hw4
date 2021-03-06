#include "mbed.h"

#include "mycar.h"
Ticker servo_ticker;
PwmOut servoL(D12), servoR(D13);
mycar car(servoL, servoR, servo_ticker);

#include "myXbee.h"
BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D1, D0);
BufferedSerial openmv(A1,A0);     //tx,rx

volatile int line_theta, line_x;
Thread t_openmv;

void lineFollow(){
    while(1){
        int goal_theta = 5 * line_x;
        int car_theta = 0;//-line_theta;
        int error = car_theta - goal_theta;
        if(error >= 180)
            error = 179;
        if(error <= -180)
            error = -179;

        double factor;
        if(error >= 0){
            factor = 1.0 / (1.0 + error / 180.0);
        }
        else{
            factor = -1.0 / (1.0 - error / 180.0);
        }
        car.turnCalib(3, factor);
    }
}

void openmv_thread(){
    int i = 0;
    char buf[16];
    while(1) {
        if(openmv.readable()){
            char recv;
            openmv.read(&recv, 1);
            //pc.write(&recv, 1);
            if(recv == '\n'){
                buf[i] = '\0';
                i = 0;
                sscanf(buf, "%d %d", &line_theta, &line_x);
                sprintf(buf, "%4d %4d\r\n", line_theta, line_x);
                xbee.write(buf, 11);
            }
            else{
                buf[i++] = recv;
            }
        }
    }
}

#include "mycar.h"
using namespace std::chrono;
DigitalIn encoder(D10);
Ticker encoder_ticker;
volatile int steps;
volatile int last;
void encoder_control(){
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

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

RPCFunction myrpc3(&rpc_goBySec, "goBySec");

int main() {
    double pwm_table0[] = {-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
    double speed_table0[] = {-16.264, -15.947, -15.947, -15.947, -15.628, -14.990, -14.352, -13.395, -12.438, -10.525, -8.611, -6.379, -4.146, -1.834, -0.000, 0.558, 2.551, 4.943, 7.017, 9.249, 10.844, 12.438, 13.714, 14.352, 14.990, 15.628, 15.947, 15.947, 16.266, 16.266, 16.266};
    double speed_table1[] = {-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.870, -0.717, 0.000, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584};
    car.setCalibTable(31, pwm_table0, speed_table0, 31, pwm_table0, speed_table1);
    car.stopCalib();

    encoder_ticker.attach(&encoder_control, 10ms);

    xbee_init();

    openmv.set_baud(9600);
    t_openmv.start(openmv_thread);

    lineFollow();

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