.POSIX:
.SUFFIXES:

# Will fix this after the jam, I promise
CFLAGS = -std=c89 -Wall -pedantic -D_XOPEN_SOURCE=600 -g -Wno-deprecated-declarations
LDFLAGS = -lX11 -lm
CC = cc

SRC = main.c gfx.c game.c worldgen.c battle.c
OBJ = ${SRC:.c=.o}

all: game

game: ${OBJ}
	${CC} ${LDFLAGS} -o $@ ${OBJ}

main.o: main.c gfx.h config.h
gfx.o: gfx.c gfx.h
game.o: game.c state.h config.h gfx.h
worldgen.o: worldgen.c config.h state.h
battle.o: battle.c

config.h: config.def.h
	cp config.def.h config.h

clean:
	rm -f ${OBJ} game

full-clean: clean
	rm -f config.h

.SUFFIXES: .c .o

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<

.PHONY: all clean full-clean
