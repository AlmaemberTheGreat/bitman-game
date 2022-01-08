#include <stdio.h>

#include "config.h"

#include "gfx.h"

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
	for (;;) {
		/* Draw the basic UI */
		mkline(w, 0, TOP_BAR_HEIGHT, WND_WIDTH, TOP_BAR_HEIGHT);

		/* Flush the window so it displays everything */
		wflush(w);
		for (;;) ;
	}
}
