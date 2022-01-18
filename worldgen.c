#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "config.h"

#include "state.h"

struct Room {
	unsigned x1, y1, x2, y2;
	int active;
};

static void drvline(unsigned x, unsigned y1, unsigned y2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx, Enemy *enemies);
static void drhline(unsigned y, unsigned x1, unsigned x2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx, Enemy *enemies);

void genwld(unsigned w, unsigned h, void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx, Enemy *enemies)
{
	unsigned nrooms;
	unsigned i, j, k;
	struct Room *rooms;
	unsigned enemies_used = 0;
	
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

				if (enemies_used < (N_ENEMIES / 2) && rand() % 3 == 0) {
					sq.t = SQ_ENEMY;
					enemies_used++;
					sq.data = &enemies[+enemies_used];
					/*                 ^ just for fun */
					(enemies + enemies_used)->nbits = rand() % 15 + 1;
					(enemies + enemies_used)->bits = rand() % UINT_MAX;
					/* we don't have access to the board directly, so we'll set the `sq` field
					in game.c */
				}

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
				drvline(tx, sy, ty, set, ctx, enemies);
			} else {
				drvline(tx, ty, sy, set, ctx, enemies);
			}
			if (tx > sx) {
				drhline(sy, sx, tx, set, ctx, enemies);
			} else {
				drhline(sy, tx, sx, set, ctx, enemies);
			}
		} else {
			if (ty > sy) {
				drvline(sx, sy, ty, set, ctx, enemies);
			} else {
				drvline(sx, ty, sy, set, ctx, enemies);
			}
			if (tx > sx) {
				drhline(ty, sx, tx, set, ctx, enemies);
			} else {
				drhline(ty, tx, sx, set, ctx, enemies);
			}
		}
	}
	free(rooms);
}

static void drvline(unsigned x, unsigned y1, unsigned y2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx, Enemy *enemies)
{
	unsigned i;
	Square sq = {SQ_EMPTY};
	unsigned enemies_used = N_ENEMIES / 2;

	for (i = y1; i < y2; ++i) {
		if (enemies_used < N_ENEMIES && rand() % 3 == 0) {
			sq.t = SQ_ENEMY;
			enemies_used++;
			sq.data = &enemies[+enemies_used];
			/*                 ^ just for fun */
			(enemies + enemies_used)->nbits = rand() % 15 + 1;
			(enemies + enemies_used)->bits = rand() % UINT_MAX;
			/* we don't have access to the board directly, so we'll set the `sq` field
			in game.c */
			(enemies + enemies_used)->sq = NULL;
		} else {
			sq.t = SQ_EMPTY;
		}
		set(x, i, sq, ctx);
	}
}

static void drhline(unsigned y, unsigned x1, unsigned x2,
                    void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx, Enemy *enemies)
{
	unsigned i;
	Square sq = {SQ_EMPTY};

	for (i = x1; i < x2; ++i) {
		set(i, y, sq, ctx);
	}
}
