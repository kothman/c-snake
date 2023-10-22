#!/bin/sh

gcc -o snake main.c -Wall -ansi `sdl2-config --cflags --libs`
