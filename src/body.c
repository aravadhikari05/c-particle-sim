#include "body.h"

struct body body_create(float mass, float radius, struct vec2 position, struct vec2 velocity) {
  struct body self = {
    .mass = mass,
    .radius = radius,
    .position = position,
    .velocity = velocity,
    .acceleration = 0.0f
  };
  return self;
}
