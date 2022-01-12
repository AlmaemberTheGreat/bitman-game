#include "state.h"
#include "gfx.h"
#include "config.h"

void genwld(unsigned w, unsigned h, void (*set)(unsigned x, unsigned y, Square sq, void *ctx), void *ctx);

static char *ops[] = {
	"OR",
	"XOR",
	"AND"
};

static Square board[N_X_PAGES][N_Y_PAGES][H_SQ_PER_WND][V_SQ_PER_WND];
size_t cur_page_x, cur_page_y;

static void renderboard(Win *w);
static void setfield(unsigned x, unsigned y, Square sq, void *ctx);

void gloop(Win *w)
{
	GameState gs = {
		1, /* alive */
		BO_XOR, /* start with XOR */
		1, 1, /* start at game coordinates 1 1 */
	};
	Square (*boardp)[N_X_PAGES][N_Y_PAGES][H_SQ_PER_WND][V_SQ_PER_WND] = &board;

	genwld(N_X_PAGES*H_SQ_PER_WND, N_Y_PAGES*V_SQ_PER_WND, setfield, boardp);

	for (;;) {
		/* Draw the basic UI */
		mkline(w, 0, TOP_BAR_HEIGHT, WND_WIDTH, TOP_BAR_HEIGHT);
		mktext(w, 30, TOP_BAR_HEIGHT/2, FONT, ops[gs.op]);

		renderboard(w);

		/* Flush the window so it displays everything */
		wflush(w);
	}
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
