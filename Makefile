CC=gcc
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -Wall -Wextra $(SDL_CFLAGS)
LDFLAGS = $(SDL_LDFLAGS)
C_STD=c99

PROGRAM = snake

all: snake

%.o: %.c types.h
	$(CC) -c -o $@ $< $(CFLAGS)

snake: main.o
	$(CC) -o $(PROGRAM) main.o $(LDFLAGS) -std=$(C_STD)

debug: clean main.o
	$(CC) -ggdb -o $(PROGRAM)_debug main.o $(CFLAGS) $(LDFLAGS) -std=$(C_STD)

clean:
	rm -f $(PROGRAM) $(PROGRAM)_debug *.o