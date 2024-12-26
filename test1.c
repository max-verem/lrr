#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char** argv)
{
    int r, fd, i;
    unsigned char buf[32];
    const char* serial_dev = "/dev/ttyS1";
    struct termios newtio;

    /* open serial port */
    fd = open(serial_dev, O_RDWR | O_NOCTTY );

    /* check if port opened */
    if(fd < 0)
    {
        r = errno;
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

//    write(fd, "^U", 2);

    while(1)
    {
        r = read(fd, buf, sizeof(buf));
        fprintf(stderr, "[%d]:", r);
        for(i = 0; i < r; i++)
            fprintf(stderr, " %.2X", buf[i]);
        fprintf(stderr, "\n");
    };

    return 0;
};
