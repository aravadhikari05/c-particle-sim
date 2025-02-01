#pragma once
#include "math.h"
#include "body.h"

typedef struct collision_data {
  bool hit;
  float depth;
  vec2 normal;
} collision_data;

typedef struct body {
  float mass;
  float radius;
  vec2 pos;
  vec2 vel;
  vec2 acc;
} body;


body body_create(float mass, float radius, vec2 pos, vec2 vel);

void body_update(body* body, double dt, int drag, vec2 magnet);

collision_data body_to_body(body body_a, body body_b);

void handle_body_collision(body* A, body* B, float e);

void handle_border_collision(body* A, float e, int WIN_W, int WIN_H);

