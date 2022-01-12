#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"

#include "gfx.h"

void gloop(Win *w);

int main()
{
	Win *w;

	srand(time(NULL));

	w = newwin(WND_WIDTH, WND_HEIGHT, "Bitman");
	wflush(w);
	gloop(w);
	delwin(w);
	return 0;
}
