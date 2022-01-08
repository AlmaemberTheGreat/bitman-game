.POSIX:
.SUFFIXES:

CFLAGS = -std=c89 -Wall -pedantic -D_XOPEN_SOURCE=600
LDFLAGS = -lX11 -lm
CC = cc

SRC = main.c gfx.c
OBJ = ${SRC:.c=.o}

all: game

game: ${OBJ}
	${CC} ${LDFLAGS} -o $@ ${OBJ}

main.o: main.c gfx.h
gfx.o: gfx.c gfx.h

config.h: config.def.h
	cp config.def.h config.h

clean:
	rm -f ${OBJ} game

.SUFFIXES: .c .o

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<

.PHONY: all clean
