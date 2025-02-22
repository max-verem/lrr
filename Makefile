TARGETS=lrr_emul lrr_read lrr_read_stream_now_cmp stm32_read_stream_now_cmp

DEBUG=-g3 -ggdb3 -O0 -D_DEBUG_

CFLAGS=$(DEBUG) -I. -Wall
LDFLAGS=$(DEBUG) -ldl -lpthread -lm

all: $(TARGETS)

%: %.c gitversion.h
	$(CC) -o $@ $< $(LDFLAGS)

./gitversion.h:
	echo -e "#ifndef GITVERSION_H\n#define GITVERSION_H\n#define GITVERSION \""`git describe --always 2> /dev/null`"\"\n#endif\n" > ./gitversion.h

clean:
	rm -f \
        gitversion.h \
        $(DEPS) \
        $(TARGETS)
