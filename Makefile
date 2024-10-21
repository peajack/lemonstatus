NAME	= lemonstatus

PREFIX	= /usr/local

SOURCE	= $(NAME).c
OBJECT	= $(NAME).o
HEADER	= $(NAME).h config.h
TARGET	= $(NAME)

MANSRC  = $(NAME).1.scd
MANTRGT = $(NAME).1

CC		= cc
CFLAGS	= -Wall -Wextra -O2 -std=c99 -pedantic -I/usr/X11R6/include
LDFLAGS	= -L/usr/X11R6/lib -lX11 -lxkbfile

.SUFFIXES: .c .o

all: $(TARGET) $(MANTRGT)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECT)

$(OBJECT): $(HEADER)

$(MANTRGT): $(MANSRC)
	scdoc < $(MANSRC) > $(MANTRGT)

clean:
	rm -f $(OBJECT) $(TARGET) $(MANTRGT)

install: $(TARGET) $(MANTRGT)
	mkdir -p $(PREFIX)/bin $(PREFIX)/share/man/man1
	cp -f $(TARGET) $(PREFIX)/bin/
	cp $(MANTRGT) $(PREFIX)/share/man/man1/

.PHONY: all clean install

include config.mk
