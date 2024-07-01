#include "draw.h"
#include "shader.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct vec2 world_to_screen(struct vec2 a) {

  a.x /= ((WIN_W - 1) / 2);
  a.y /= ((WIN_H - 1) / 2);

  a.x -= 1;
  a.y -= 1;
  
  a.y = -a.y;
  return a;
}


static struct vec4 normalize_rgba(struct vec4 color) {
  color.x /= 255.0f;
  color.y /= 255.0f;
  color.z /= 255.0f;
  return color;
}

void draw_point(GLuint shader_program, GLuint VAO, int x0, int y0, struct vec4 color, float size) {
  struct vec2 a = vec2_create((float) x0, (float) y0);
  a = world_to_screen(a);
  color = normalize_rgba(color);

  glBindVertexArray(VAO);
  glUseProgram(shader_program);
  set_uniform_2f(shader_program, "translation", a.x, a.y);
  set_uniform_4f(shader_program, "color", color.x, color.y, color.z, color.w);

  glPointSize(size);
  glDrawArrays(GL_POINTS, 0, 1);
}

float* circle_generate_verts(float radius, int num_verts) {
  float* verts = malloc(sizeof(float) * num_verts * 2);
  float angle = 360.f/num_verts;
  double deg_to_rad = M_PI/180;
  for (int i = 0; i < num_verts; i++) {
    float current_angle = (float) (i * angle * deg_to_rad);
    struct vec2 pos = vec2_create((float) (radius * cos(current_angle)), (float) (radius * sin(current_angle)));
    verts[2 * i] = pos.x;
    verts[2 * i + 1] = pos.y;
  }
  return verts;
}

unsigned int* circle_generate_indices(int num_verts) {
  unsigned int* indices = malloc(sizeof(unsigned int) * (num_verts - 2) * 3);
  for (int i = 0; i < num_verts - 2; i++) {
    indices[3 * i] = 0;
    indices[3 * i + 1] = i + 1;
    indices[3 * i + 2] = i + 2; 
  }
  return indices;
}
void draw_circle(GLuint shader_program, GLuint VAO, struct vec2 pos, float scale, struct vec4 color, int num_elements) {
  pos = world_to_screen(pos);
  color = normalize_rgba(color);

  glBindVertexArray(VAO);
  glUseProgram(shader_program);
  set_uniform_2f(shader_program, "translation", pos.x, pos.y);
  set_uniform_2f(shader_program, "scale", scale, scale);
  set_uniform_4f(shader_program, "color", color.x, color.y, color.z, color.w);

  glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (void*) 0); 
}
