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

#include "ctrl_chars.h"
#include "serial_open.h"

#include "gitversion.h"

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

    if(argc == 1)
        serial_dev = "/dev/ttyUSB0";
    else
        serial_dev = argv[1];

    fprintf(stderr, "Will use [%s] serial device (pass another as arg if needed)\n", serial_dev);

    /* open serial port */
    fd = serial_open(serial_dev);
    if(fd < 0)
        return -1;

    /* <ctrl>+S X-OFF terminates reporting.*/
    write(fd, &ctrl_s, 1);
    sleep(1);
    tcflush(fd, TCIFLUSH);

    while(1)
    {
        now(buf, sizeof(buf));
        fprintf(stderr, "\n@ %s\n", buf);

        /* <ctrl>+R Report Time Address, User Groups and Status as selected. */
        write(fd, &ctrl_r, 1);
        sleep(1);

        r = read(fd, buf, sizeof(buf));
        fprintf(stderr, "[%d]:", r);
        for(i = 0; i < r; i++)
            fprintf(stderr, " %.2X", buf[i]);
        fprintf(stderr, "\n> ");
        for(i = 0; i < r; i++)
            fprintf(stderr, "%c", buf[i]);
        fprintf(stderr, "\n");

        sleep(1);
    };

    return 0;
};
