#include <stdlib.h>
#include <string.h>

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

void delwin(Win *w)
{
	XUnmapWindow(w->dpy, w->w);
	XCloseDisplay(w->dpy);
	free(w);
}

void wflush(Win *w)
{
	XFlush(w->dpy);
}

void mkline(Win *w, int x1, int y1, int x2, int y2)
{
	XDrawLine(w->dpy, w->w, w->gc, x1, y1, x2, y2);
}

void mktext(Win *w, int x, int y, Font font, char *txt)
{
	XTextItem item;
	item.chars = txt;
	item.nchars = strlen(txt);
	item.delta = 0;
	item.font = font;

	/* I have no idea how or what this does */
	XDrawText(w->dpy, w->w, w->gc, x, y, &item, 1);
}
