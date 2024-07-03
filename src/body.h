#pragma once
#include "math.h"

struct body {
  float mass;
  float radius;
  struct vec2 pos;
  struct vec2 vel;
  struct vec2 acc;
};

struct body body_create(float mass, float radius, struct vec2 pos, struct vec2 vel);

void body_update(struct body* body, double dt, int drag);
