#include <X11/Xlib.h>

typedef struct Win {
	Display *dpy;
	Window w;
	GC gc;

	int black, white;
} Win;

Win *newwin(unsigned width, unsigned height, char *capt);
void delwin(Win *w);
void wflush(Win *w);
void wclear(Win *w);

void mkline(Win *w, int x1, int y1, int x2, int y2);
void mkrect(Win *w, int x1, int y1, unsigned wd, unsigned h);
void mkfrec(Win *w, int x1, int y1, unsigned wd, unsigned h);
void mktext(Win *w, int x, int y, Font font, char *txt);
