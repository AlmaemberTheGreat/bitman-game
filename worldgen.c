#include <stdlib.h>

#include "config.h"

#include "state.h"

struct Room {
	unsigned x1, y1, x2, y2;
	int active;
};

void genwld(unsigned w, unsigned h, void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx)
{
	unsigned nrooms;
	unsigned i, j, k;
	struct Room *rooms;
	
	/* Create rooms */
	nrooms = rand() % MAX_ROOMS;

	rooms = calloc(nrooms + 1, sizeof(struct Room));
	if (!rooms) exit(EXIT_FAILURE);

	rooms[0].x1 = 1;
	rooms[0].y1 = 1;
	rooms[0].x2 = 4;
	rooms[0].y2 = 4;
	rooms[0].active = 1;

	for (i = 1; i < nrooms + 1; ++i) {
		struct Room r;
		unsigned rw, rh;
	retry:
		rw = (rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH)) + MIN_ROOM_WIDTH;
		rh = (rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT)) + MIN_ROOM_HEIGHT;

		r.x1 = rand() % w;
		r.y1 = rand() % h;
		r.x2 = r.x1 + rw;
		r.y2 = r.y1 + rh;
		if (r.x2 > w || r.y2 > h) {
			if (rw != MIN_ROOM_WIDTH && rh != MIN_ROOM_HEIGHT) goto retry;
			r.active = 0;
		} else {
			r.active = 1;
		}

		rooms[i] = r;
	}

	/* Render the rooms */
	for (i = 0; i < nrooms + 1; ++i) {
		if (!rooms[i].active) continue;
		for (j = rooms[i].x1; j < rooms[i].x2; ++j) {
			for (k = rooms[i].y1; k < rooms[i].y2; ++k) {
				Square sq = {SQ_EMPTY};
				set(j, k, sq, ctx);
			}
		}
	}

	free(rooms);
}
