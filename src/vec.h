#pragma once

typedef struct vec2 {
  float x;
  float y;
} vec2;

typedef struct vec3 {
  float x;
  float y;
  float z;
} vec3;

typedef struct vec4 {
  float x;
  float y;
  float z;
  float w;
} vec4;

vec2 vec2_create(float x, float y);

vec2 vec2_zero(void);

struct vec4 vec4_create(float x, float y, float z, float w);

vec2 vec2_add(vec2 a, vec2 b);

vec2 vec2_sub(vec2 a, vec2 b);

vec2 vec2_mult(vec2 a, vec2 b);

float vec2_dot(vec2 a, vec2 b);

float vec2_mag(vec2 a); 

vec2 vec2_unit(vec2 a);

vec2 vec2_inv(vec2 a);

vec2 vec2_scale(vec2 a, float s);

