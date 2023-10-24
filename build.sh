#!/bin/sh

gcc -o snake main.c -Wall -Wextra -ansi `sdl2-config --cflags --libs`
