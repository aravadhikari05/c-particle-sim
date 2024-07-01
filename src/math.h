#pragma once

struct vec2 {
  float x;
  float y;
};

struct vec3 {
  float x;
  float y;
  float z;
};

struct vec4 {
  float x;
  float y;
  float z;
  float w;
};

struct vec2 vec2_create(float x, float y);

struct vec4 vec4_create(float x, float y, float z, float w);

struct vec2 vec2_add(struct vec2 a, struct vec2 b);

struct vec2 vec2_multiply(struct vec2 a, struct vec2 b);

float vec2_dot(struct vec2 a, struct vec2 b);

struct vec2 vec2_inverse(struct vec2 a);

struct vec2 vec2_scalar(struct vec2 a, float s);

