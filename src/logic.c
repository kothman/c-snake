#include "logic.h"
#include "types.h"

bool is_mouse_over_exit_button(int x, int y)
{
  if (x >= MENU_EXIT_BUTTON_X && x < MENU_EXIT_BUTTON_X + MENU_EXIT_BUTTON_W &&
      y >= MENU_EXIT_BUTTON_Y && y < MENU_EXIT_BUTTON_Y + MENU_EXIT_BUTTON_H)
    return true;
  else
    return false;
}

bool is_point_in_array(Point *p, Point *p_arr, unsigned int length)
{
  for (unsigned int i = 0; i < length; i++) {
    if ((p_arr + i)->x == p->x && (p_arr + i)->y == p->y)
      return true;
  }
  return false;
}
