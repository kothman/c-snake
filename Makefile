#
# A makefile for managing the build of snakeV2 C source files
#
# Referencing:
# https://github.com/gwu-cs-os/evening_os_hour/blob/master/f19/10.2-makefiles/03_featureful_makefile/Makefile
# https://www.youtube.com/watch?v=DtGrdB8wQ_8

# C Compiler
CC = gcc

# where the C source files and headers are located
SRC_DIR = ./src

# name of our program executable
PROGRAM_NAME = snake

# gcc flags to generate files that encode make rules for the .h dependencies
DEPFLAGS = -MP -MD

# CFLAGS variable sets the compiler flags
#  -g			compile with debug info
#  -Wall 		include verbose compiler warnings
#  -Wextra		extra warning flags not enabled by -Wall
#  -Wpedantic		issue warnings demanded by strict ISO C standard
#  -O0			don't optimize the code
#  -std=c99		use the C99 standard, because why not
#  -I/usr/include/SDL2	include the SDL2 header files
#  -D_REENTRANT		required for SDL2, ensure use of thread-safe functions and types
CFLAGS = -g -Wall -Wextra -Wpedantic -O0 -std=c99 -I/usr/include/SDL2 -D_REENTRANT $(DEPFLAGS)

# LDFLAGS variable sets the linker flags
#  -lSDL2 include the SDL2 for dynamic linking
LDFLAGS = -lSDL2

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES=$(foreach D,$(SRC_DIR),$(wildcard $(D)/*.c))

# pattern matching replacement
# # pathsubst replaces everything in $(CFILES) matching %.c with %.o or %.d
# # https://www.gnu.org/software/make/manual/html_node/Text-Functions.html 
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
# expected dependency files, based on existing .c files
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# preprocess, compile, and assemble all .c files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./$(PROGRAM_NAME)

clean:
	rm -fr $(PROGRAM_NAME) $(OBJECTS) $(DEPFILES)

# required for make to include the dependencies
-include $(DEPFILES)