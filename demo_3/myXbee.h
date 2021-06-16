extern BufferedSerial xbee;

void reply_messange(char *xbee_reply, char *messange);
void check_addr(char *xbee_reply, char *messenger);
void xbee_rx_interrupt(void);
void xbee_rx(void);
void xbee_init();