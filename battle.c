#include <time.h>
#include <stdio.h>
#include <X11/keysym.h>
#include <stdlib.h>

#include "state.h"
#include "gfx.h"
#include "config.h"

static char *ops[] = {
	"OR",
	"XOR",
	"AND"
};

void battle(Win *w, Enemy e, GameState *gs)
{
	time_t start_time;
	char goal[64] = {0}, enemy_code[64] = {0}, input_buf[64] = 
		"            ";
	size_t input_buf_pos = 12;
	XEvent ev;

	start_time = time(NULL);

	snprintf(goal, 64,       "      Goal: %024x Op: %s", gs->goal, ops[gs->op]);
	snprintf(enemy_code, 64, "Enemy code: %024x", e.bits);

	while (1) {
		wclear(w);
		mkfrec(w, 100, 100, WND_WIDTH / 2 - 200, WND_HEIGHT - 200);
		mktext(w, 100, 90, None,
		       "You, playing against...");
		mkrect(w, WND_WIDTH / 2 + 100, 100,
		       WND_WIDTH / 2 - 200, WND_HEIGHT - 200);
		mktext(w, WND_WIDTH / 2 + 150, 250, None,
		       "this random cube!");
		
		mktext(w, WND_WIDTH / 2 - 100, WND_HEIGHT - 50, None, goal);
		mktext(w, WND_WIDTH / 2 - 100, WND_HEIGHT - 40, None, enemy_code);
		mktext(w, WND_WIDTH / 2 - 100, WND_HEIGHT - 30, None, input_buf);
		wflush(w);

		XNextEvent(w->dpy, &ev);
		if (ev.type == KeyPress) {
			KeySym ks;
			unsigned usrnum, usrres;

			ks = XKeycodeToKeysym(w->dpy, ev.xkey.keycode, 0);

			switch (ks) {
			case XK_0:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '0'; break;
			case XK_1:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '1'; break;
			case XK_2:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '2'; break;
			case XK_3:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '3'; break;
			case XK_4:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '4'; break;
			case XK_5:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '5'; break;
			case XK_6:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '6'; break;
			case XK_7:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '7'; break;
			case XK_8:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '8'; break;
			case XK_9:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = '9'; break;
			case XK_a:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'a'; break;
			case XK_b:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'b'; break;
			case XK_c:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'c'; break;
			case XK_d:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'd'; break;
			case XK_e:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'e'; break;
			case XK_f:	if (input_buf_pos < 63) input_buf[input_buf_pos++] = 'f'; break;

			case XK_BackSpace: if (input_buf_pos > 12) input_buf[--input_buf_pos] = '\0';
			                   else input_buf[12] = '\0';
			                   break;
			}

			sscanf(input_buf, " %x ", &usrnum);
			switch (gs->op) {
			case BO_XOR: usrres = e.bits ^ usrnum; break;
			case BO_OR:  usrres = e.bits | usrnum; break;
			case BO_AND: usrres = e.bits & usrnum; break;
			}
			if (time(NULL) - start_time > BATTLE_SECONDS) {
				printf("TIMEOUT!\n");
				goto fail;
			}
			if (usrres == gs->goal) {
				/* win */
				return;
			} else {
				if (time(NULL) - start_time <= BATTLE_SECONDS) continue;
			}
				/* fail */
			fail:
				wclear(w);
				delwin(w);
				printf("You lost :(\nSo close, right?\n");
				exit(0);
		}
	}	
}
