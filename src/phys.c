#include "game.h"
#include <math.h>
#include <stdio.h>

static vec2 apply_forces(body body, int drag, vec2 magnet);
//collide
body body_create(float mass, float radius, vec2 pos, vec2 vel) {
  body self = {
    .mass = mass,
    .radius = radius,
    .pos = pos,
    .vel = vel,
    .acc = vec2_create(0.0f, 0.0f),
  };
  return self;
}

void body_update(body* body, double dt, int drag, vec2 magnet) {
  vec2 new_pos = vec2_add(vec2_add(body->pos, vec2_scale(body->vel, dt)), vec2_scale(body->acc, dt * dt * 0.5));
  vec2 new_acc = apply_forces(*body, drag, magnet);
  vec2 new_vel = vec2_add(body->vel, vec2_scale(vec2_add(body->acc, new_acc), dt * 0.5));
  body->pos = new_pos;
  body->vel = new_vel;
  body->acc = new_acc;
}

collision_data body_to_body(body body_a, body body_b) {
	collision_data out;
  out.hit = false;
	vec2 d = vec2_sub(body_b.pos, body_a.pos);
	float d2 = vec2_dot(d, d);
	float r = body_a.radius + body_b.radius;
	if (d2 < r * r) {
		float l = vec2_mag(d);
		d = l == 0 ? vec2_create(0, 1) : vec2_scale(d, (1.0f / l));
		out.hit = true;
		out.depth = r - l;
		out.normal = d;
	}
	return out;
}

static vec2 apply_forces(body body, int drag, vec2 magnet) {
  vec2 grav_acc = vec2_create(0.0f, 0.0f);
  vec2 drag_acc = vec2_zero();
  vec2 magnetic_acc = vec2_zero();
  if (drag) {
    vec2 drag_force = vec2_scale(vec2_mult(body.vel, body.vel), (0.01 * body.radius));
    drag_acc = vec2_scale(drag_force, (1.0f / body.mass) * ((body.vel.y < 0) - (body.vel.y > 0)));  
  } 
  if (magnet.x != 0 || magnet.y != 0) {
    //vec2 magnetic_force = vec2_scale(vec2_unit(magnet), 100000.0f / (vec2_mag(magnet) * vec2_mag(magnet)));
    vec2 magnetic_force = vec2_scale(vec2_unit(magnet), 1000);
    magnetic_acc = vec2_scale(magnetic_force, 1.0f);
  }
  return vec2_add(vec2_add(grav_acc, drag_acc), magnetic_acc);
}

void handle_body_collision(body* A, body* B, float e) {
	collision_data col = body_to_body(*A, *B);

  if (!col.hit) return;

  //TODO implement dt
  float inv_mass_A = 1.0f / A->mass;
  float inv_mass_B = 1.0f / B->mass;

  vec2 rv = vec2_sub(B->vel, A->vel);

  float vel_along_normal = vec2_dot(rv, col.normal);
  if(vel_along_normal > 0) {
    return;
  }

  float j = -(1 + e) * vel_along_normal;
  j /= inv_mass_A + inv_mass_B;

  vec2 impulse = vec2_scale(col.normal, j);
  A->vel = vec2_sub(A->vel, vec2_scale(impulse, inv_mass_A));
  B->vel = vec2_add(B->vel, vec2_scale(impulse, inv_mass_B));
}

void handle_border_collision(body* A, float e, int WIN_W, int WIN_H) {
  if (A->pos.y + A->radius >= WIN_H - 1) {
    A->vel.y = -A->vel.y * e;
  } else if (A->pos.y - A->radius <= 0) { 
    A->vel.y = -A->vel.y * e;
  } 
  if (A->pos.x + A->radius >= WIN_W - 1) { 
   A->vel.x = -A->vel.x * e;
  } else if (A->pos.x - A->radius <= 0) { 
    A->vel.x = -A->vel.x * e;
  } 
}



/*TODO 
 
 *create a physics world that can be translated to screen world
 *create standard units
 *terminal velocity
 *real gravity

*/


