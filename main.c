#include <stdio.h>

#include "config.h"

#include "gfx.h"

void gloop(Win *w);

int main()
{
	Win *w;

	w = newwin(WND_WIDTH, WND_HEIGHT, "Bitman");
	wflush(w);
	gloop(w);
	delwin(w);
	return 0;
}
