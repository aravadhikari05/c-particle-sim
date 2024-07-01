#include "math.h"


struct vec2 vec2_create(float x, float y) {
  struct vec2 self = {
    .x = x,
    .y = y
  };
  return self;
}

struct vec4 vec4_create(float x, float y, float z, float w) {
  struct vec4 self = {
    .x = x,
    .y = y,
    .z = z,
    .w = w
  };
  return self;
}

struct vec2 vec2_add(struct vec2 a, struct vec2 b) {
  return vec2_create(a.x + b.x, a.y + b.y);
}

struct vec2 vec2_multiply(struct vec2 a, struct vec2 b) {
  return vec2_create(a.x * b.x, a.y * b.y);
}

float vec2_dot(struct vec2 a, struct vec2 b) {
  return a.x * b.x + a.y * b.y;
}

struct vec2 vec2_inverse(struct vec2 a) {
  return vec2_create(1 / a.x, 1 / a.y);
}

struct vec2 vec2_scalar(struct vec2 a, float s) {
  return vec2_create(s * a.x, s * a.y);
}
 




