/*

    gcc lrr_read.c -o lrr_read

*/

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
#include <sys/time.h>

static int now(char* buf, int limit)
{
    int f;
    struct tm t;
    time_t timep;
    struct timeval arrived;

    gettimeofday(&arrived, NULL);
    timep = arrived.tv_sec;
    localtime_r(&timep, &t);
    f = arrived.tv_usec / 40000LL;

    snprintf(buf, limit, "%.2d:%.2d:%.2d:%.2d +0000\r", t.tm_hour, t.tm_min, t.tm_sec, f);

    return strlen(buf);
}


int main(int argc, char** argv)
{
    int r, fd, i;
    char buf[128];
    const char* serial_dev;
    struct termios newtio;

    if(argc == 1)
        serial_dev = "/dev/ttyUSB0";
    else
        serial_dev = argv[1];

    fprintf(stderr, "Will use [%s] serial device (pass another as arg if needed)\n", serial_dev);

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
        unsigned char ctrl_r = 0x12;


        now(buf, sizeof(buf));
        fprintf(stderr, "\n****** %s\n", buf);

        write(fd, &ctrl_r, 1);
        sleep(1);

        r = read(fd, buf, sizeof(buf));
        fprintf(stderr, "[%d]:", r);
        for(i = 0; i < r; i++)
            fprintf(stderr, " %.2X", buf[i]);
        fprintf(stderr, "\n###### ");
        for(i = 0; i < r; i++)
            fprintf(stderr, "%c", buf[i]);
        fprintf(stderr, "\n");

        sleep(1);
    };

    return 0;
};