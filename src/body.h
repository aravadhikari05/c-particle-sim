#pragma once
#include "math.h"

struct body {
  float mass;
  float radius;
  struct vec2 position;
};

struct body body_create(float mass, float radius, struct vec2 position);

