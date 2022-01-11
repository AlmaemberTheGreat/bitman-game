#include <stdio.h>

#include "config.h"

#include "gfx.h"
#include "state.h"

static char *ops[] = {
	"OR",
	"XOR",
	"AND"
};

static void gloop(Win *w);

int main()
{
	Win *w;

	w = newwin(WND_WIDTH, WND_HEIGHT, "Bitman");
	wflush(w);
	gloop(w);
	delwin(w);
	return 0;
}

static void gloop(Win *w)
{
	GameState gs = {
		1, /* alive */
		BO_XOR, /* start with XOR */
		0, 0, /* start at game coordinates 0 0 */
	};

	for (;;) {
		/* Draw the basic UI */
		mkline(w, 0, TOP_BAR_HEIGHT, WND_WIDTH, TOP_BAR_HEIGHT);
		mktext(w, 30, TOP_BAR_HEIGHT/2, FONT, ops[gs.op]);

		/* Flush the window so it displays everything */
		wflush(w);
		for (;;) ;
	}
}
