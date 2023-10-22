#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

/**
 * Only valid to call if an SDL function has signaled an error, otherwise
 * might still return error string on success
 **/
void print_sdl_err() {
  fprintf(stderr, "[SDL error]: %s\n", SDL_GetError());
  SDL_ClearError();
}

int main()
{
  bool should_quit = false;
  int screen_width = 640;
  int screen_height = 480;
  
  SDL_Window * window = NULL;
  SDL_Surface * surface = NULL;

  SDL_Event e;
  
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("Sssssssssssssnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    screen_width, screen_height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  surface = SDL_GetWindowSurface(window);
  if (surface == NULL) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  while (!should_quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type){
      case SDL_QUIT:
	should_quit = true;
	break;
      default:
	break;
      }
    }
  }
  
  SDL_Quit();
  return EXIT_SUCCESS;
}
