TARGETS=lrr_emul lrr_read lrr_read_stream_now_cmp stm32_read_stream_now_cmp

DEBUG=-g3 -ggdb3 -O0 -D_DEBUG_

CFLAGS=$(DEBUG) -I. -Wall
LDFLAGS=$(DEBUG) -ldl -lpthread -lm

all: $(TARGETS)

%: %.c
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -f \
        gitversion.h \
        $(DEPS) \
        $(TARGETS)
