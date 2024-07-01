#pragma once
#include "math.h"

struct body {
  float mass;
  float radius;
  struct vec2 position;
  struct vec2 velocity;
  struct vec2 acceleration;
};

struct body body_create(float mass, float radius, struct vec2 position, struct vec2 velocity);

