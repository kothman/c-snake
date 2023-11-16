#include <stdbool.h>
#include "constants.h"
#include "types.h"

#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

bool is_mouse_over_exit_button(int x, int y);
bool is_point_in_array(Point *p, Point *p_arr, unsigned int length);

#endif /* SNAKE_LOGIC_H */
