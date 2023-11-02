CC=gcc
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -Wall -Wextra $(SDL_CFLAGS)
LDFLAGS = $(SDL_LDFLAGS)

PROGRAM = snake
TARGET = main.c


all: snake

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

snake: main.o
	$(CC) -o $(PROGRAM) main.o $(LDFLAGS)

debug: clean
	$(CC) -ggdb -o $(PROGRAM)_debug $(TARGET) $(LDFLAGS)

clean:
	rm -f $(PROGRAM) $(PROGRAM)_debug *.o