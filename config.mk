PREFIX = /usr
MANPREFIX = $(PREFIX)/share/man

CC = cc
LD = $(CC)

CFLAGS += -std=c99 -pedantic -Wall -Weverything -Os
LDFLAGS += -lxcb
