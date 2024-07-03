#include "body.h"

struct body body_create(float mass, float radius, struct vec2 pos, struct vec2 vel) {
  struct body self = {
    .mass = mass,
    .radius = radius,
    .pos = pos,
    .vel = vel,
    .acc = vec2_create(0, 500.0f)
  };
  return self;
}

static struct vec2 apply_forces(struct body body) {
  return vec2_create(0.0, 500);
}

void body_update(struct body* body, double dt) {
  struct vec2 new_pos = vec2_add(vec2_add(body->pos, vec2_scale(body->vel, dt)), vec2_scale(body->acc, dt * dt * 0.5));
  struct vec2 new_acc = apply_forces(*body);
  struct vec2 new_vel = vec2_add(body->vel, vec2_scale(vec2_add(body->acc, new_acc), dt * 0.5));
  body->pos = new_pos;
  body->vel = new_vel;
  body->acc = new_acc;
}

