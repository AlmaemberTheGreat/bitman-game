#include <stdlib.h>

#include <X11/Xlib.h>

#include "gfx.h"

/* Open a new window. Returns NULL if failed */
Win *newwin(unsigned width, unsigned height, char *capt)
{
	Win *w;
	Display *dpy;

	w = malloc(sizeof(*w));
	if (!w) return NULL;

	dpy = XOpenDisplay(NULL);
	if (!dpy) return NULL;
	w->dpy = dpy;

	w->black = BlackPixel(dpy, DefaultScreen(dpy));
	w->white = WhitePixel(dpy, DefaultScreen(dpy));

	w->w = XCreateSimpleWindow(
		dpy,
		DefaultRootWindow(dpy),
		0, 0, width, height, 0,
		w->black, w->white
	);

	/* From below here, I don't really know what's going on */
	XSelectInput(dpy, w->w, StructureNotifyMask);
	XMapWindow(dpy, w->w);
	w->gc = XCreateGC(w->dpy, w->w, 0, NULL);
	XSetForeground(w->dpy, w->gc, w->black);

	for (;;) {
		/* Asynchronous programming before its time? */
		XEvent e;
		XNextEvent(dpy, &e);
		if (e.type == MapNotify)
			break;
	}

	XStoreName(w->dpy, w->w, capt);

	return w;
}
