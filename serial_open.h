#ifndef serial_open_h
#define serial_open_h

static int serial_open(const char* serial_dev)
{
    int fd;
    struct termios newtio;

    /* open serial port */
    fd = open(serial_dev, O_RDWR | O_NOCTTY );

    /* check if port opened */
    if(fd < 0)
    {
        int r = errno;
        fprintf(stderr, "%s: failed to open [%s], r=%d (%s)\n",
            __FUNCTION__, serial_dev, r, strerror(r));
        return -r;
    };

    /* configure serial device */
    memset(&newtio, 0, sizeof(struct termios));     /* clear struct for new port settings */
    newtio.c_iflag = IGNPAR;                        /* ignore bytes with parity errors */
    newtio.c_oflag = 0;                             /* Raw output */
    newtio.c_lflag = 0;                             /* enable canonical input */
    newtio.c_cc[VTIME] = 1;                         /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;                          /* blocking read until 5 chars received */
    newtio.c_cflag =
        B9600 |
//        PARENB |
//        PARODD |
        CS8 |
        CREAD |
        CLOCAL;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    /* discard all data in buffers */
    tcflush(fd, TCIOFLUSH);

    return fd;
};

#endif
