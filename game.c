#include "state.h"
#include "gfx.h"
#include "config.h"

static char *ops[] = {
	"OR",
	"XOR",
	"AND"
};

void gloop(Win *w)
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
	}
}
