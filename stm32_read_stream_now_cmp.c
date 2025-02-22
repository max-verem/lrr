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

static char* now_tc(struct timeval *arrived, char* buf, int limit)
{
    int f;
    struct tm t;
    time_t timep;

    timep = arrived->tv_sec;
    localtime_r(&timep, &t);
    f = arrived->tv_usec / 40000LL;

    snprintf(buf, limit, "%.2d:%.2d:%.2d:%.2d", t.tm_hour, t.tm_min, t.tm_sec, f);

    return buf;
}

#define BUF_LEN 128
int main(int argc, char** argv)
{
    int r, fd, i, s;
    char buf[BUF_LEN + 1] = {0};
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

    while(1)
    {
        char c = 0;

        read(fd, &c, 1);

        if(c == '\n' || c == '\r')
        {
            if(s)
            {
                buf[s] = 0;
                fprintf(stderr, "\n> [%s] {%d}\n", buf, s);
                if(s == 11)
                {
                    int hh = 0, mm = 0, ss = 0, ff = 0;

                    if(4 == sscanf(buf, "%d:%d:%d:%d", &hh, &mm, &ss, &ff))
                    {
                        char tmp[32];
                        int now_ms, ltc_ms;
                        struct timeval arrived;

                        gettimeofday(&arrived, NULL);

                        ltc_ms = ss * 1000 + ff * 40;
                        now_ms = (arrived.tv_sec % 60) * 1000 + arrived.tv_usec / 1000LL;

                        fprintf(stderr, "@ [%s]\n", now_tc(&arrived, tmp, sizeof(tmp)));
                        fprintf(stderr, "~ %5d ms\n", now_ms - ltc_ms);
                    };
                };
            };

            s = 0;
        }
        else
        {
            if(s < BUF_LEN)
            {
                buf[s] = c;
                s++;
            }
            else
                s = 0;
        };
    };

    return 0;
};
