## LITTLE RED LTC utilities

### lrr_read

This utility read timecode value from LRR at specific interval and compare it current local time:

```
[root@mcr-cg-elements lrr]# ./lrr_read
Will use [/dev/ttyUSB0] serial device (pass another as arg if needed)

@ 11:13:29:00 +0000
[18]: 31 31 3A 31 33 3A 32 38 3A 32 34 20 2B 30 30 30 30 0D
> 11:13:28:24 +0000

@ 11:13:31:00 +0000
[18]: 31 31 3A 31 33 3A 33 30 3A 32 34 20 2B 30 30 30 30 0D
> 11:13:30:24 +0000
^C
```

as seen, current localtime is ONE FRAME in advance, that mean that it actually in sync with LTC timecode, because LRR decoding LTC frame after recieving that is one frame after starts.

### lrr_read_stream_now_cmp

this utility read LTC values from LRR in continious mode. i.e. LRR send timecode value after it decode received LTC frame:

```
[root@mcr-cg-elements lrr]# ./lrr_read_stream_now_cmp
Will use [/dev/ttyUSB0] serial device (pass another as arg if needed)
> [11:16:39:20 +0000] {17}
@ [11:16:39:23]
~   121 ms
> [11:16:39:21 +0000] {17}
@ [11:16:39:23]
~   107 ms
> [11:16:39:22 +0000] {17}
@ [11:16:40:00]
~   120 ms
> [11:16:39:23 +0000] {17}
@ [11:16:40:00]
~   107 ms
> [11:16:39:24 +0000] {17}
@ [11:16:40:02]
~   121 ms
> [11:16:40:00 +0000] {17}
@ [11:16:40:02]
~   107 ms
> [11:16:40:01 +0000] {17}
@ [11:16:40:04]
~   121 ms
> [11:16:40:02 +0000] {17}
@ [11:16:40:04]
~   107 ms
```

Local time is **(107 + 121)/2 -  40 = 74** miliseconds ahead.
