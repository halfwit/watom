/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>
#include <string.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);
static int get_watom(xcb_window_t, char *);

static void
usage(char *name) {
	fprintf(stderr, "usage: %s <wid> <ATOM_NAME>\n", name);
	exit(1);
}


static xcb_atom_t xcb_atom_get(const char *name) {
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, (unsigned short)strlen(name), name);
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
	return !reply ? XCB_ATOM_STRING : reply->atom;
}

static int
get_watom(xcb_window_t win, char *prop) {

	const char *types[] = { "STRING", "UTF8_STRING", "COMPOUND_TEXT", "CARDINAL", "ATOM", "WM_HINTS", "WM_SIZE_HINTS", NULL};
	int len = 0;
	xcb_get_property_cookie_t cookie;
  
	for(const char **s = types; *s != NULL; s++) {
		cookie = xcb_get_property(conn, 0, win, xcb_atom_get(prop), xcb_atom_get(*s), 0L, 32L);
		xcb_get_property_reply_t *r;
		r = xcb_get_property_reply(conn, cookie, NULL);
		if (r) {
			len = xcb_get_property_value_length(r);
			if (strcmp(prop, "WM_CLASS")) {
					
			}
			if(len > 0){
				printf("%.*s\n", len, (char *) xcb_get_property_value(r));
				free(r);
				return 0;
			}
			free(r);
		}
	}
	warnx("could not get %s\n", prop);
	return 1;
}

int
main(int argc, char **argv) {
	int r = 0;
	if (argc != 3) {
		usage(argv[0]);
	}
	init_xcb(&conn);
	r = get_watom((unsigned int)strtoul(argv[1], NULL, 16), argv[2]);
	kill_xcb(&conn);
	return r;
}
