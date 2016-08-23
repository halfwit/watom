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
usage(char *name)
{
	fprintf(stderr, "usage: %s <wid> <ATOM_NAME>\n", name);
	exit(1);
}

const char *types[] = { "STRING", "UTF8_STRING", "COMPOUND_TEXT", "CARDINAL", "ATOM", "WM_HINTS", "WM_SIZE_HINTS"};

static xcb_atom_t xcb_atom_get(xcb_connection_t *conn, const char *name)
{
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen(name), name);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
  return !reply ? XCB_ATOM_STRING : reply->atom;
}

static int
get_watom(xcb_window_t win, char *prop)
{
  int len = 0;
  xcb_get_property_cookie_t cookie;
  
  size_t i;
  for(i = 0; i < sizeof(types) / sizeof(char *); i++) {
    cookie = xcb_get_property(conn, 0, win,
      xcb_atom_get(conn, prop),
      xcb_atom_get(conn, types[i]),
      0L, 32L);
    xcb_get_property_reply_t *r;
    r = xcb_get_property_reply(conn, cookie, NULL);
  
    if (r) {
      len = xcb_get_property_value_length(r);
    
      if(len > 0){
        printf("%.*s\n", len, (char *) xcb_get_property_value(r));
        free(r);
        return 0;
      }
    }
    free(r);
  }

  warnx("could not get %s\n", prop);
  return 1;
}

int
main(int argc, char **argv)
{
	int r = 0;

	if (argc != 3)
		usage(argv[0]);

	init_xcb(&conn);

	r = get_watom(strtoul(argv[1], NULL, 16), argv[2]);

	kill_xcb(&conn);

	return r;
}
