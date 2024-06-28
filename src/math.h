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

struct vec2 vec2_negate(struct vec2 a);

struct vec3 vec2_to_vec3(struct vec2 a);
