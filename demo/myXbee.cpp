#include "mbed.h"
#include "myXbee.h"

BufferedSerial xbee(D1, D0);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

void reply_messange(char *xbee_reply, const char *messange){
    xbee.read(&xbee_reply[0], 1);
    xbee.read(&xbee_reply[1], 1);
    xbee.read(&xbee_reply[2], 1);
    if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
        printf("%s\r\n", messange);
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
        xbee_reply[2] = '\0';
    }
}

void check_addr(char *xbee_reply, char *messenger){
   xbee.read(&xbee_reply[0], 1);
   xbee.read(&xbee_reply[1], 1);
   xbee.read(&xbee_reply[2], 1);
   xbee.read(&xbee_reply[3], 1);
   printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);
   xbee_reply[0] = '\0';
   xbee_reply[1] = '\0';
   xbee_reply[2] = '\0';
   xbee_reply[3] = '\0';
}

void xbee_rx(void){
    static int i = 0;
    static char buf[128];
    while(xbee.readable()){
        char c;
        xbee.read(&c, 1);
        if(c!='\r' && c!='\n'){
            buf[i] = c;
            i++;
        }else if(i == 0){ // ignore redundant char in buffer

        }
        else{
            buf[i] = '\0';
            printf("Get: %s\n", buf);
            xbee.write(buf, sizeof(buf));
            i = 0;
        }
    }
    ThisThread::sleep_for(100ms);
}

void xbee_rx_interrupt(void){
    queue.call(&xbee_rx);
}

void xbee_init(){
    char xbee_reply[4];
    xbee.set_baud(9600);
    xbee.write("+++", 3);
    xbee.read(&xbee_reply[0], sizeof(char));
    xbee.read(&xbee_reply[1], sizeof(char));
    if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){
        printf("enter AT mode.\r\n");
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
    }
    xbee.write("ATMY 0x220\r\n", 12);
    reply_messange(xbee_reply, "setting MY : 0x220");
    xbee.write("ATDL 0x120\r\n", 12);
    reply_messange(xbee_reply, "setting DL : 0x120");
    xbee.write("ATID 0x1\r\n", 10);
    reply_messange(xbee_reply, "setting PAN ID : 0x1");
    xbee.write("ATWR\r\n", 6);
    reply_messange(xbee_reply, "write config");
    xbee.write("ATMY\r\n", 6);
    check_addr(xbee_reply, "MY");
    xbee.write("ATDL\r\n", 6);
    check_addr(xbee_reply, "DL");
    xbee.write("ATCN\r\n", 6);
    reply_messange(xbee_reply, "exit AT mode");
    while(xbee.readable()) {
        char k;
        xbee.read(&k, 1);
        printf("clear\r\n");
    }
    printf("start\r\n");
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    xbee.set_blocking(false);
    xbee.sigio(mbed_event_queue()->event(xbee_rx_interrupt));
}