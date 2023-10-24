# C Snake
A classic snake game, written (poorly) in C

### Requirements
libsdl2-dev, libsdl2-2.0-0 ([Simple DirectMedia Layer](https://www.libsdl.org/))

On a Debian OS, like Ubuntu, install these packages with `sudo apt install libsdl2-dev libsdl2-2.0.0`

To build, run the build script `./build.sh`, then start the game with `./snake`

### Controls
Arrow keys to move, Q / ESC to quit, P / Space to pause

Press P or Space to start new game if you've collided into the boundaries or yourself

### Features
* Aesthetically pleasing minimalistic design; no distracting music or score counter
* Snake moves faster after eating food
* Snake gets bigger after eating food
* Snake dies if it crashes into itself or the window boundaries



### To Do
* Reduce redundancy in commonly performed functions, like rendering
* Move certain parameters to global variables, like grid_cells_count_x and grid_cells_count_y
* Allow window resizing with preserved aspect ratio
* Add more error handling, for SDL functions and malloc/realloc