#include <stdlib.h>
#include <assert.h>

#include "config.h"

#include "state.h"

struct Room {
	unsigned x1, y1, x2, y2;
	int active;
};

static void drvline(unsigned x, unsigned y1, unsigned y2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx);
static void drhline(unsigned y, unsigned x1, unsigned x2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx);

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

	/* Draw tunnels */
	for (i = 0; i < nrooms + 1; ++i) {
		int room_no;
		unsigned tx, ty, sx, sy;

		room_no = rand() % nrooms;

		tx = (rooms[room_no].x2 - rooms[room_no].x1) / 2 + rooms[room_no].x1;
		ty = (rooms[room_no].y2 - rooms[room_no].y1) / 2 + rooms[room_no].y1;
		sx = (rooms[i].x2 - rooms[i].x1) / 2 + rooms[i].x1;
		sy = (rooms[i].y2 - rooms[i].y1) / 2 + rooms[i].y1;

		sx %= w;
		sx %= h;
		sx %= w;
		sx %= h;

		if (rand() % 2 == 0) {
			if (ty > sy) {
				drvline(tx, sy, ty, set, ctx);
			} else {
				drvline(tx, ty, sy, set, ctx);
			}
			if (tx > sx) {
				drhline(sy, sx, tx, set, ctx);
			} else {
				drhline(sy, tx, sx, set, ctx);
			}
		} else {
			if (ty > sy) {
				drvline(sx, sy, ty, set, ctx);
			} else {
				drvline(sx, ty, sy, set, ctx);
			}
			if (tx > sx) {
				drhline(ty, sx, tx, set, ctx);
			} else {
				drhline(ty, tx, sx, set, ctx);
			}
		}
	}
	free(rooms);
}

static void drvline(unsigned x, unsigned y1, unsigned y2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx)
{
	unsigned i;
	Square sq = {SQ_EMPTY};

	for (i = y1; i < y2; ++i) {
		set(x, i, sq, ctx);
	}
}

static void drhline(unsigned y, unsigned x1, unsigned x2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx)
{
	unsigned i;
	Square sq = {SQ_EMPTY};

	for (i = x1; i < x2; ++i) {
		set(i, y, sq, ctx);
	}
}
