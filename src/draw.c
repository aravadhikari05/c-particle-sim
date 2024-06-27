#include "draw.h"

struct vec2 world_to_screen(struct vec2 a) {

  a.x /= (SCREEN_WIDTH / 2);
  a.y /= (SCREEN_HEIGHT / 2);

  a.x -= 1;
  a.y -= 1;
  
  a.y = -a.y;
  return a;
}


