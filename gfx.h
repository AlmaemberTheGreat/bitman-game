#include <X11/Xlib.h>

typedef struct Win {
	Display *dpy;
	Window w;
	GC gc;

	int black, white;
} Win;

Win *newwin(unsigned width, unsigned height, char *capt);
