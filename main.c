#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#define INITIAL_SNAKE_SPEED_MS 250;
#define INITIAL_SNAKE_LENGTH 5;
#define INITIAL_SNAKE_DIRECTION EAST;

/**
 * Defines the x and y coordinates of a cell, for the snake body and food pieces.
 * 
 * Top left corner is (0, 0), incrementing down and to the right.
*/
typedef struct {
  short x, y;
} Cell;

/**
 * Directions that the snake can move in
*/
enum Direction {
  NORTH, SOUTH, EAST, WEST
};

/**
 * Only valid to call if an SDL function has signaled an error, otherwise
 * might still return error string on success
 **/
void print_sdl_err() {
  fprintf(stderr, "[SDL error]: %s\n", SDL_GetError());
  SDL_ClearError();
}

void clear_renderer(SDL_Renderer * renderer)
{
  if (SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE) < 0) {
    print_sdl_err();
  }
  if (SDL_RenderClear(renderer) < 0) {
    print_sdl_err();
  }
}

bool x_y_is_in_cell_array(Cell * c, short c_length, short x, short y)
{
  int i;
  for (i = 0; i < c_length; i++) {
    if (c[i].x == x && c[i].y == y)
      return true;
  }
  return false;
}

void randomize_food_location(Cell * food, Cell * snake_body, short snake_body_length, short grid_cells_count_x, short grid_cells_count_y)
{
  short x, y;
  /**
   * Probably a more efficient way to make sure food locaiton is not within snake body,
   * but probably not too inefficient given small size of snake array.
   * 
   * What happens if the food can't be rendered anywhere?
   **/
  do {
    x = rand() % grid_cells_count_x;
    y = rand() % grid_cells_count_y;
  } while (x_y_is_in_cell_array(snake_body, snake_body_length, x, y));
  (*food).x = x;
  (*food).y = y;
}

void render_snake(SDL_Renderer * renderer, Cell * snake_body, short snake_body_length,
                  short screen_width, short screen_height, short grid_cells_count_x, short grid_cells_count_y)
{
  int i;
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
  /** Calculate size and position of each cell based on window and grid size
   * 
   * This code is duplicated in the render_food function, and can probably be consolidated
  */
  short cell_size_x = screen_width / grid_cells_count_x;
  short cell_size_y = screen_height / grid_cells_count_y;
  for (i = 0; i < snake_body_length; i++) {
    SDL_Rect rect = {
      snake_body[i].x * cell_size_x,
      snake_body[i].y * cell_size_y,
      cell_size_x,
      cell_size_y
    };
    SDL_RenderFillRect(renderer, &rect);
  }
}

void render_food(SDL_Renderer * renderer, Cell * food, short screen_width, short screen_height,
                 short grid_cells_count_x, short grid_cells_count_y)
{
  short cell_size_x = screen_width / grid_cells_count_x;
  short cell_size_y = screen_height / grid_cells_count_y;
  SDL_Rect food_rect = {food->x * cell_size_x, food->y * cell_size_y, cell_size_x, cell_size_y};
  SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &food_rect);
}


void init_snake(Cell * snake_body, short * snake_body_length, short grid_cells_count_y) {
  int i;
  *snake_body_length = INITIAL_SNAKE_LENGTH;
  snake_body = realloc(snake_body, *snake_body_length);
  /**
   * Initialize the snake_body cell locations, starting from the West and centered vertically.
   * Assumes that the snake is moving East, and the first element in snake_body is the head.
   **/
  for (i = 0; i < *snake_body_length; i++) {
    snake_body[i].x = (*snake_body_length) - i - 1;
    snake_body[i].y = grid_cells_count_y / 2;
  }
}

/* Having to include grid_cells_count_y here is silly, but I'm not sure I want to make grid_cells_count_x|y global? */
void kill_snake(bool * paused, Cell * snake_body, short * snake_body_length, short grid_cells_count_y) {
  (*paused) = true;
  init_snake(snake_body, snake_body_length, grid_cells_count_y);
}


int main()
{
  int i;

  bool should_quit = false;
  bool paused = false;
  bool has_drawn_pause_screen = false;

  short screen_width = 640;
  short screen_height = 480;

  /** How many cells exist in the player's grid */
  short grid_cells_count_x = 40;
  short grid_cells_count_y = 30;

  enum Direction snake_direction = INITIAL_SNAKE_DIRECTION;
  short snake_body_length = INITIAL_SNAKE_LENGTH;

  /** Last cell in array is the snake's tail */
  Cell * snake_body = malloc(snake_body_length * sizeof(Cell));

  short snake_move_speed_ms = INITIAL_SNAKE_SPEED_MS;
  Uint64 last_snake_move_ms;

  Cell food = {0, 0};

  Uint64 game_loop_start;
  Uint64 game_loop_end;

  init_snake(snake_body, &snake_body_length, grid_cells_count_y);
  /* Initialize rand() for our food randomizer function; can this be done within the following function, only once, instead? */
  srand(time(0));
  randomize_food_location(&food, snake_body, snake_body_length, grid_cells_count_x, grid_cells_count_y);

  SDL_Window * window = NULL;
  SDL_Renderer * renderer = NULL;

  SDL_Event e;
  
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("Sssssssssssssnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    screen_width, screen_height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    print_sdl_err();
    return EXIT_FAILURE;
  }

  /* Enable alpha blending, for pause screen overlay*/
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  clear_renderer(renderer);
  render_snake(renderer, snake_body, snake_body_length, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
  render_food(renderer, &food, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
  SDL_RenderPresent(renderer);
  last_snake_move_ms = SDL_GetTicks64();
  while (!should_quit) {
    game_loop_start = SDL_GetPerformanceCounter();
    /** Handle button presses and keystrokes */
    while (SDL_PollEvent(&e)) {
      switch (e.type){
      case SDL_QUIT:
	      should_quit = true;
	      break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
          case SDLK_UP:
            if (snake_direction != SOUTH)
              snake_direction = NORTH;
            break;
          case SDLK_DOWN:
            if (snake_direction != NORTH)
              snake_direction = SOUTH;
            break;
          case SDLK_LEFT:
            if (snake_direction != EAST)
              snake_direction = WEST;
            break;
          case SDLK_RIGHT:
            if (snake_direction != WEST)
              snake_direction = EAST;
            break;
          case SDLK_SPACE:
          case SDLK_p:
            paused = !paused;
            if (paused == false) {

              /** Redraw snake without pause overlay, otherwise game looks like it lags to unpause */
              clear_renderer(renderer);
              render_snake(renderer, snake_body, snake_body_length, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
              render_food(renderer, &food, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
              /** Update screen with rendering */
              SDL_RenderPresent(renderer);
              has_drawn_pause_screen = false;

            }
            break;
          case SDLK_q:
          case SDLK_ESCAPE:
            should_quit = true;
            break;
          default:
            break;
        }
        break;
          default:
	      break;
      }
    }

    /** Check if game paused and display pause screen
     * 
     * Wait snake_move_speed_ms before moving
     *
     * Keep updating last_snake_move_ms while paused, so snake doesn't move immediately
    */
    if (paused) {
      last_snake_move_ms = SDL_GetTicks64();
      if (has_drawn_pause_screen == false) {
        clear_renderer(renderer);
        render_snake(renderer, snake_body, snake_body_length, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
        render_food(renderer, &food, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x80);
        SDL_Rect pause_overlay_rect = {0, 0, screen_width, screen_height};
        SDL_RenderFillRect(renderer, &pause_overlay_rect);
        SDL_RenderPresent(renderer);
        has_drawn_pause_screen = true;
      }

    /**
     * The snake movement logic and rendering
    */
    } else if (SDL_GetTicks64() > last_snake_move_ms + snake_move_speed_ms) {
      /* Save tail piece in case snake ate its food */
      Cell tail = { snake_body[snake_body_length - 1].x, snake_body[snake_body_length -1].y };
      for (i = snake_body_length - 1; i >= 0; i--) {
        if (i == 0) {
          switch (snake_direction) {
            case NORTH:
              --snake_body[i].y;
              break;
            case SOUTH:
              ++snake_body[i].y;
              break;
            case EAST:
              ++snake_body[i].x;
              break;
            case WEST:
              --snake_body[i].x;
              break;
            default:
              fprintf(stderr, "[error]: snake_direction (%i) unexpected?\n", snake_direction);
              break;
          }
        } else {
          snake_body[i].x = snake_body[i-1].x;
          snake_body[i].y = snake_body[i-1].y;
        }

      }

      /* Has the snake bumped into itself or the boundaries of the window? */
      if (snake_body[0].x < 0 || snake_body[0].x >= grid_cells_count_x || snake_body[0].y < 0 || snake_body[0].y >= grid_cells_count_y ||
          /* First element of snake_body is head, and we don't want to check if the head exists at its own location */
          x_y_is_in_cell_array(snake_body + 1, snake_body_length - 1, snake_body->x, snake_body->y)) {
        /* Snake is ded :( */
        kill_snake(&paused, snake_body, &snake_body_length, grid_cells_count_y);
        randomize_food_location(&food, snake_body, snake_body_length, grid_cells_count_x, grid_cells_count_y);
        snake_direction = INITIAL_SNAKE_DIRECTION
        snake_move_speed_ms = INITIAL_SNAKE_SPEED_MS;
      }

      /* Did the snake eat its food? */
      if (snake_body[0].x == food.x && snake_body[0].y == food.y) {
        /* Increment and reallocate + 1 snake segment */
        snake_body = realloc(snake_body, (++snake_body_length) * sizeof(Cell));
        /* Restore the tail */
        snake_body[snake_body_length - 1].x = tail.x;
        snake_body[snake_body_length - 1].y = tail.y;
        randomize_food_location(&food, snake_body, snake_body_length, grid_cells_count_x, grid_cells_count_y);
        /* Make snake go fast */
        if (snake_move_speed_ms > 10) {
          snake_move_speed_ms -= 4;
        }
      }

      /** Draw the snake and food*/
      clear_renderer(renderer);
      render_snake(renderer, snake_body, snake_body_length, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
      render_food(renderer, &food, screen_width, screen_height, grid_cells_count_x, grid_cells_count_y);
      /** Update screen with rendering */
      SDL_RenderPresent(renderer);

      last_snake_move_ms = SDL_GetTicks64();
    }
    
    /**
     * https://thenumb.at/cpp-course/sdl2/08/08.html
     * Frequency = Count / Seconds
     * PerformanCounter / Frequency = Seconds
     **/
    game_loop_end = SDL_GetPerformanceCounter();
    float elapsedMS = ((game_loop_end - game_loop_start) / (float) SDL_GetPerformanceFrequency()) * 1000.0f;
    SDL_Delay((int) (33.333f - elapsedMS));
  }

  free(snake_body);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
