#include "body.h"

struct body body_create(float mass, float radius, struct vec2 position) {
  struct body self = {
    .mass = mass,
    .radius = radius,
    .position = position
  };
  return self;
}
