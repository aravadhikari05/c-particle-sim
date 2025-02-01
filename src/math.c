#include "math.h"
#include "stdio.h"
#include <math.h>


vec2 vec2_create(float x, float y) {
  vec2 self = {
    .x = x,
    .y = y
  };
  return self;
}

vec2 vec2_zero(void) {
  return vec2_create(0.0f, 0.0f);
}

vec4 vec4_create(float x, float y, float z, float w) {
  vec4 self = {
    .x = x,
    .y = y,
    .z = z,
    .w = w
  };
  return self;
}

vec2 vec2_add(vec2 a, vec2 b) {
  return vec2_create(a.x + b.x, a.y + b.y);
}

vec2 vec2_sub(vec2 a, vec2 b) {
  return vec2_create(a.x - b.x, a.y - b.y);
}

vec2 vec2_mult(vec2 a, vec2 b) {
  return vec2_create(a.x * b.x, a.y * b.y);
}

float vec2_dot(vec2 a, vec2 b) {
  return a.x * b.x + a.y * b.y;
}

float vec2_mag(vec2 a) {
  return sqrt(vec2_dot(a, a));
}

vec2 vec2_unit(vec2 a) {
  return vec2_scale(a, 1.0f / vec2_mag(a));
}

vec2 vec2_inv(vec2 a) {
  int x = a.x == 0 ? 0 : 1.0f / a.x;
  int y = a.y == 0 ? 0 : 1.0f / a.y;
  return vec2_create(x, y);
}

vec2 vec2_scale(vec2 a, float s) {
  return vec2_create(s * a.x, s * a.y);
}
