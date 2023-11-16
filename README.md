# C Snake
A classic snake game, written in C

### Requirements
* [Make](https://www.gnu.org/software/make/)
* libsdl2-dev, libsdl2-2.0-0 ([Simple DirectMedia Layer](https://www.libsdl.org/))

On a Debian OS, like Ubuntu, install these packages with `sudo apt install libsdl2-dev libsdl2-2.0.0 make`

To build the game, run the command `make` from the project root, then start the game with `./snake`

### Controls
Arrow keys to move, Q / ESC to quit, P / Space to pause

Press P or Space to start new game if you've collided into the boundaries or yourself

### Features
* Aesthetically pleasing minimalistic design; no distracting music or score counter
* Snake moves faster after eating food
* Snake gets bigger after eating food
* Snake dies if it crashes into itself or the window boundaries

### To-Do
* Add a timer and score counter to menu
* Draw an 'x' on the close button
* Standardize rounding of floats to ints when drawing grid, snake, and food; alternatively, use SDL float-specific functions