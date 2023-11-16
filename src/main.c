#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "types.h"
#include "logic.h"

/* #define DEBUG */

/* snake.c functions */
Snake * snake_initialize(void);
void snake_deinitialize(Snake *s);

/* render.c functions */
/* render function is only responsible for drawing game objects */
void render(GameState *state);

/* process_input.c functions */
/* process_input is only responsible for setting GameState based on input */
void process_input(GameState *state);

/* update.c functions */
/* update function is only responsible for handling game logic */
void update(GameState *state);

GameState * initialize()
{
  GameState *state = NULL;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  Snake *snake = NULL;
  
  /* allocate space for our GameState struct, return NULL if malloc fails */
  state = malloc(sizeof(GameState));
  if (state == NULL) {
    fprintf(stderr, "[error]: Could not allocate memory for GameState\n");
    return NULL;
  }

  /* initialize SDL subsystems, or print error and return NULL on failure */
  if (SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS ) != 0) {
    fprintf(stderr, "[error]: %s\n", SDL_GetError());
    return NULL;
  }
  
  /* allocate the SDL window */
  window = SDL_CreateWindow("snake", /* title */
					SDL_WINDOWPOS_CENTERED, /* window x position */
					SDL_WINDOWPOS_CENTERED, /* window y position */
					WINDOW_WIDTH_INITIAL, /* window width */
					WINDOW_HEIGHT_INITIAL, /* window height */
					SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS); /* window flags */
  /* print error and return NULL if SDL_CreateWindow fails */
  if (window == NULL) {
    fprintf(stderr, "[error]: %s\n", SDL_GetError());
    return NULL;
  }
  
  /* allocate the SDL renderer */
  renderer = SDL_CreateRenderer(window, /* window utilizing the rendering context */
				-1,     /* use the first available supported driver */
				SDL_RENDERER_ACCELERATED); /* SDL_RendererFlags */
  /* print error and return NULL if SDL_CreateRenderer fails */
  if (renderer == NULL) {
    fprintf(stderr, "[error]: %s\n", SDL_GetError());
    return NULL;
  }

  /* Set renderer to blend, for pause screen overlay */
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  /* initialize and allocate snake */
  snake = snake_initialize();
  if (snake == NULL) {
    fprintf(stderr, "[error]: Failed to initialize snake in snake.c:snake_initialize()\n");
    return NULL;
  }

  /* initialize rand() function for randomizing food location */
  srand(time(NULL));
  
  /* don't forget to initialize the GameState struct! */
  state->window = window;
  state->renderer = renderer;
  state->is_running = true;
  state->is_paused = false;
  state->score = 0;
  state->snake = snake;
  state->food.x = 0, state->food.y = 0;
  
  return state;
}

/* main game loop */
/* handles user input, updates game objects, and renders to the window */
/* loop function also responsible for capping FPS */
void loop(GameState *state)
{
  Uint64 frame_start_time_ms, delta_time_ms;

  #ifdef DEBUG
  Uint64 second_start_time_ms;
  unsigned int frame_counter = 0;
  second_start_time_ms = SDL_GetTicks64();
  #endif /* #ifdef DEBUG */

  while (state->is_running) {
    frame_start_time_ms = SDL_GetTicks64();
    #ifdef DEBUG 
    ++frame_counter;
    #endif
    
    process_input(state);
    update(state);
    render(state);

    delta_time_ms = SDL_GetTicks64() - frame_start_time_ms;
    if (delta_time_ms < MS_PER_FRAME)
      SDL_Delay(MS_PER_FRAME - delta_time_ms);

    #ifdef DEBUG
    if (SDL_GetTicks64() - second_start_time_ms >= 1000) {
      fprintf(stdout, "[info]: %d frames per second\n", frame_counter);
      frame_counter = 0;
      second_start_time_ms = SDL_GetTicks64();
    }
    #endif /* #ifdef DEBUG */
  }
}

/* deallocate all previously allocated resources */
/* IMPORTANT: we cannot use the function name 'shutdown', as libX11/libxcb utilize this name */
int deinitialize(GameState *state)
{
  SDL_DestroyRenderer(state->renderer);
  SDL_DestroyWindow(state->window);
  SDL_Quit();

  snake_deinitialize(state->snake);
  free(state);
  
  return 0;
}

int main(int argc, char *argv[])
{
  GameState *state = NULL;

  /* suppress compiler warnings for unused parameters argc and argv */
  if (argc != 1)
    fprintf(stdout, "[info]: %s called with %i arguments\n", argv[0], argc -1);
  
  /* setup our Window and Renderer */
  if ((state = initialize()) == NULL) {
    return EXIT_FAILURE;
  }
  
  /* the main game loop */
  loop(state);
  
  /* deallocate resources */
  if (deinitialize(state) != 0) {
    return EXIT_FAILURE;
  }
  /* program was successfully terminated */
  return EXIT_SUCCESS;
}
