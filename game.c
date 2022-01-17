#include <X11/keysym.h>

#include "state.h"
#include "gfx.h"
#include "config.h"

void genwld(unsigned w, unsigned h, void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx);

static char *ops[] = {
	"OR",
	"XOR",
	"AND"
};

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

static Square board[N_X_PAGES][N_Y_PAGES][H_SQ_PER_WND][V_SQ_PER_WND];
size_t cur_page_x, cur_page_y;

static void renderboard(Win *w);
static void setfield(unsigned x, unsigned y, Square sq, void *ctx);
static void renderplayer(Win *w, GameState *gs);
static void mvplayer(GameState *gs, enum Direction dir);

void gloop(Win *w)
{
	GameState gs = {
		1, /* alive */
		BO_XOR, /* start with XOR */
		1, 1, /* start at game coordinates 1 1 */
	};
	Square (*boardp)[N_X_PAGES][N_Y_PAGES][H_SQ_PER_WND][V_SQ_PER_WND] = &board;
	XEvent e;

	genwld(N_X_PAGES*H_SQ_PER_WND, N_Y_PAGES*V_SQ_PER_WND, setfield, boardp);

	for (;;) {
		wclear(w);

		/* Draw the basic UI */
		mkline(w, 0, TOP_BAR_HEIGHT, WND_WIDTH, TOP_BAR_HEIGHT);
		mktext(w, 30, TOP_BAR_HEIGHT/2, FONT, ops[gs.op]);

		renderboard(w);
		renderplayer(w, &gs);

		/* Flush the window so it displays everything */
		wflush(w);

		XNextEvent(w->dpy, &e);
		if (e.type == KeyPress) {
			KeySym ks;

			ks = XKeycodeToKeysym(w->dpy, e.xkey.keycode, 0);
			switch (ks) {
			case XK_Escape:
				goto after_loop;
			case XK_Up:
				mvplayer(&gs, UP);
				break;
			case XK_Down:
				mvplayer(&gs, DOWN);
				break;
			case XK_Right:
				mvplayer(&gs, RIGHT);
				break;
			case XK_Left:
				mvplayer(&gs, LEFT);
				break;
			}
		}
	}
	after_loop:;
}

static void renderboard(Win *w)
{
	size_t i, j;
	Square *sq;

	for (i = 0; i < H_SQ_PER_WND; ++i) {
		if (ENABLE_GRID) mkline(w, SQ_WIDTH*i, TOP_BAR_HEIGHT, SQ_WIDTH*i, WND_HEIGHT);

		for (j = 0; j < V_SQ_PER_WND; ++j) {
			if (ENABLE_GRID) mkline(w, 0, TOP_BAR_HEIGHT+SQ_HEIGHT*j, WND_WIDTH, TOP_BAR_HEIGHT+SQ_HEIGHT*j);

			sq = &(board[cur_page_x][cur_page_y][i][j]);

			switch (sq->t) {
			case SQ_WALL: {
				mkfrec(w, SQ_WIDTH*i, TOP_BAR_HEIGHT+SQ_HEIGHT*j, SQ_WIDTH, SQ_HEIGHT);
				break;
			}

			case SQ_ENEMY: {
				break;
			}

			case SQ_EMPTY: break;
			}
		}
	}
}

static void setfield(unsigned x, unsigned y, Square sq, void *ctx)
{
	unsigned pagex, offsetx, pagey, offsety;
	Square (*boardp)[N_X_PAGES][N_Y_PAGES][H_SQ_PER_WND][V_SQ_PER_WND];

	pagex = x / H_SQ_PER_WND;
	offsetx = x % H_SQ_PER_WND;
	pagey = y / V_SQ_PER_WND;
	offsety = y % V_SQ_PER_WND;

	boardp = ctx;

	/* I'm sorry */
	(*boardp)[pagex][pagey][offsetx][offsety] = sq;
}

static void renderplayer(Win *w, GameState *gs)
{
	int realx, realy;

	realx = SQ_WIDTH * gs->x;
	realy = (SQ_HEIGHT * gs->y) + TOP_BAR_HEIGHT;

	mkfrec(w, realx+5, realy+5, SQ_WIDTH-10, SQ_HEIGHT-10);
}

static void mvplayer(GameState *gs, enum Direction dir)
{
	int newx, newy;
	size_t newpagex, newpagey;

	newx = gs->x;
	newy = gs->y;
	newpagex = cur_page_x;
	newpagey = cur_page_y;
	switch (dir) {
	case UP:    newy -= 1; break;
	case DOWN:  newy += 1; break;
	case LEFT:  newx -= 1; break;
	case RIGHT: newx += 1; break;
	}

	if (newx > H_SQ_PER_WND) {
		newx = 0;
		newpagex += 1;
	}
	if (newy > V_SQ_PER_WND) {
		newy = 0;
		newpagey += 1;
	}
	if (newx < 0) {
		newx = H_SQ_PER_WND - 1;
		if (!newpagex) newpagex -= 1;
		else return;
	}
	if (newy < 0) {
		newy = V_SQ_PER_WND - 1;
		if (!newpagey) newpagey -= 1;
		else return;
	}

	if (newpagex > N_X_PAGES - 1 || newpagey > N_Y_PAGES - 1) return;

	if (board[newpagex][newpagey][newx][newy].t == SQ_WALL) return;

	gs->x = newx;
	gs->y = newy;
	cur_page_x = newpagex;
	cur_page_y = newpagey;
}
