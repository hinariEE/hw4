#include "mbed.h"
using namespace std::chrono;
DigitalOut testled(LED1);

#include "mycar.h"
Ticker servo_ticker;
PwmOut servoL(D12), servoR(D13);
mycar car(servoL, servoR, servo_ticker);

#include "myXbee.h"
BufferedSerial xbee(D1, D0);

BufferedSerial openmv(A1, A0); // -> (P5, P4)

volatile int line_theta, line_x;

void openmv_thread_hw4_2(){
    int i = 0;
    char buf[16];
    while(1) {
        if(openmv.readable()){
            char recv;
            openmv.read(&recv, 1);
            if(recv != '\n'){
                buf[i++] = recv;
            }
            else{
                buf[i] = '\0';
                i = 0;
                sscanf(buf, "%d %d", &line_theta, &line_x);
                sprintf(buf, "%4d %4d\r\n", line_theta, line_x);
                xbee.write(buf, 11);
            }
        }
    }
}

void lineFollow(){
    while(1){
        int goal_theta = 5 * line_x;
        int car_theta = 0;//-line_theta; // not suitable because of the viewpoint of the camera
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

void rpc_hw4_2(Arguments *in, Reply *out){
    Thread t_openmv;
    t_openmv.start(openmv_thread_hw4_2);
    lineFollow();
    t_openmv.terminate();
}

RPCFunction myrpc_hw4_2(&rpc_hw4_2, "hw4_2");

DigitalInOut ping(D2);
Ticker ping_ticker;
Timer ping_timer;
volatile float pingDist;
volatile int headingAngle;
volatile int xAprilTag;

void pingMeas(){
    ping.output();
    ping = 0; wait_us(200);
    ping = 1; wait_us(5);
    ping = 0; wait_us(5);

    ping.input();
    while(ping.read() == 0);
    ping_timer.start();
    while(ping.read() == 1);
    float ToF = ping_timer.read();
    pingDist = ToF * 17700.4f;
    ping_timer.stop();
    ping_timer.reset();
}

void openmv_thread_hw4_3(){
    int i = 0;
    char buf[32];
    while(1) {
        if(openmv.readable()){
            char recv;
            openmv.read(&recv, 1);
            if(recv != '\n'){
                buf[i++] = recv;
            }
            else{
                buf[i] = '\0';
                i = 0;
                sscanf(buf, "%d %d", &headingAngle, &xAprilTag);
                xAprilTag -= 7;
                sprintf(buf, "%4d %4d %5.1f\r\n", headingAngle, xAprilTag, pingDist);
                xbee.write(buf, 17);
            }
        }
    }
}

void posCalib(){
    Timer timer;
    bool converging = false;
    while(1){
        car.spinCalib(-xAprilTag / 10.0);
        if(!converging){
            if(abs(xAprilTag) < 6){
                timer.start();
                converging = true;
            }
        }
        else{
            if(abs(xAprilTag) < 6){
                if(duration_cast<seconds>(timer.elapsed_time()).count() > 1){
                    timer.stop();
                    timer.reset();
                    break;
                }
            }
            else{
                timer.stop();
                timer.reset();
                converging = false;
            }
        }
    }
    car.stopCalib();
    ThisThread::sleep_for(1s);

    double headingAngle_2 = 2.5 * headingAngle;
    if(headingAngle_2 >= 90.0)
        headingAngle_2 = 80.0;
    else if(headingAngle_2 <= -90.0)
        headingAngle_2 = -80.0;
    double goal_x = pingDist * sin(headingAngle / 180.0 * 3.14);
    double path = goal_x / sin(headingAngle_2 / 180.0 * 3.14);
    car.spinDeg(headingAngle - headingAngle_2);
    car.goByCm(path);

    car.spinDeg(headingAngle_2);

    while(1){
        car.spinCalib(-xAprilTag / 10.0);
        if(!converging){
            if(abs(xAprilTag) < 6){
                timer.start();
                converging = true;
            }
        }
        else{
            if(abs(xAprilTag) < 6){
                if(duration_cast<seconds>(timer.elapsed_time()).count() > 1){
                    timer.stop();
                    timer.reset();
                    break;
                }
            }
            else{
                timer.stop();
                timer.reset();
                converging = false;
            }
        }
    }
    car.stopCalib();
}

void rpc_hw4_3(Arguments *in, Reply *out){
    Thread t_openmv;
    t_openmv.start(openmv_thread_hw4_3);
    posCalib();
    t_openmv.terminate();
    xbee.write("\n", 1);
}

RPCFunction myrpc_hw4_3(&rpc_hw4_3, "hw4_3");

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
    car.stopCalib();
    ThisThread::sleep_for(2500ms);
    char buf[16];
    sprintf(buf, "%7.3f", (float)steps * 6.5 * 3.14 / 32);
    out->putData(buf);
}

RPCFunction myrpc3(&rpc_goBySec, "goBySec");



int main()
{
    double pwm_table0[] = {-150, -140, -130, -120, -110, -100, -90, -80, -70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
    double speed_table0[] = {-15.626, -15.628, -15.309, -14.990, -14.990, -14.352, -13.714, -12.758, -12.119, -10.206, -8.292, -6.060, -3.827, -1.834, -0.000, 0.638, 2.631, 4.943, 7.017, 9.249, 10.844, 12.119, 13.395, 14.033, 14.671, 14.990, 15.309, 15.309, 15.628, 15.628, 15.628};
    //double speed_table0[] = {-16.264, -15.947, -15.947, -15.947, -15.628, -14.990, -14.352, -13.395, -12.438, -10.525, -8.611, -6.379, -4.146, -1.834, -0.000, 0.558, 2.551, 4.943, 7.017, 9.249, 10.844, 12.438, 13.714, 14.352, 14.990, 15.628, 15.947, 15.947, 16.266, 16.266, 16.266};
    double speed_table1[] = {-15.626, -15.628, -15.628, -14.990, -15.309, -14.671, -14.352, -13.714, -12.757, -11.163, -9.887, -7.335, -5.422, -3.109, -0.638, 0.000, 2.073, 4.225, 6.379, 8.292, 10.525, 12.119, 13.395, 13.714, 14.352, 14.990, 14.990, 15.309, 15.309, 15.628, 15.628};
    //double speed_table1[] = {-16.264, -16.266, -16.265, -15.947, -15.627, -15.309, -14.671, -14.033, -13.076, -11.481, -9.568, -7.335, -5.103, -2.870, -0.717, 0.000, 2.392, 4.624, 7.016, 9.249, 10.843, 12.438, 13.714, 14.671, 15.309, 15.627, 15.946, 16.266, 16.265, 16.266, 16.584};
    car.setCalibTable(31, pwm_table0, speed_table0, 31, pwm_table0, speed_table1);
    car.stopCalib();

    encoder_ticker.attach(&encoder_control, 10ms);
    ping_ticker.attach(&pingMeas, 500ms);
    openmv.set_baud(9600);
    xbee_init();

    BufferedSerial pc(USBTX, USBRX);
    FILE *devin = fdopen(&pc, "r");
    char buf[128], outbuf[256] = {0};
    while(1){
        unsigned i;
        for(i = 0; i < sizeof(buf) - 1; i++){
            char recv = fgetc(devin);
            if(recv == '\n') {
                break;
            }
            buf[i] = putchar(recv);
        }
        buf[i++] = '\0';
        putchar('\n');
        RPC::call(buf, outbuf);
        printf("%s\n", outbuf);
    }
}