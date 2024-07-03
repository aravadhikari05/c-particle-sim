#include "body.h"

struct body body_create(float mass, float radius, struct vec2 pos, struct vec2 vel) {
  struct body self = {
    .mass = mass,
    .radius = radius,
    .pos = pos,
    .vel = vel,
    .acc = vec2_create(0.0f, 500.0f)
  };
  return self;
}

static struct vec2 apply_forces(struct body body, int drag) {
  struct vec2 grav_acc = vec2_create(0.0f, 500.0f);
  struct vec2 drag_force = vec2_scale(vec2_mult(body.vel, body.vel), (0.0001 * body.radius));
  struct vec2 drag_acc = vec2_scale(drag_force, 1.0f / body.mass); 

  if (drag) {
    return grav_acc;
  } 
  
  if (body.vel.y > 0) {
    return vec2_sub(grav_acc, drag_acc);
  } else {
    return vec2_add(grav_acc, drag_acc);
  }
}

void body_update(struct body* body, double dt, int drag) {
  struct vec2 new_pos = vec2_add(vec2_add(body->pos, vec2_scale(body->vel, dt)), vec2_scale(body->acc, dt * dt * 0.5));
  struct vec2 new_acc = apply_forces(*body, drag);
  struct vec2 new_vel = vec2_add(body->vel, vec2_scale(vec2_add(body->acc, new_acc), dt * 0.5));
  body->pos = new_pos;
  body->vel = new_vel;
  body->acc = new_acc;
}

