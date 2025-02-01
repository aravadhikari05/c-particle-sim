#include "draw.h"
#include "shader.h"
#include "math.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//Objects
GLuint bo_create_and_store(GLenum type, size_t size, void* data, bool dynamic) {
  GLuint bo;

  glGenBuffers(1, &bo); 
  glBindBuffer(type, bo);
  glBufferData(type, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  return bo;
}

GLuint vao_create_and_bind(void) {
  GLuint vao;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  return vao;
}

void vao_attrib(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);  
}

//Drawing
vec2 world_to_screen(vec2 a) {

  a.x /= ((WIN_W - 1) / 2);
  a.y /= ((WIN_H - 1) / 2);

  a.x -= 1;
  a.y -= 1;
  
  a.y = -a.y;
  return a;
}


static vec4 normalize_rgba(vec4 color) {
  color.x /= 255.0f;
  color.y /= 255.0f;
  color.z /= 255.0f;
  return color;
}

void draw_point(GLuint shader_program, GLuint vao, int x0, int y0, vec4 color, float size) {
  vec2 a = vec2_create((float) x0, (float) y0);
  a = world_to_screen(a);
  color = normalize_rgba(color);

  glBindVertexArray(vao);
  glUseProgram(shader_program);
  set_uniform_2f(shader_program, "translation", a.x, a.y);
  set_uniform_4f(shader_program, "color", color.x, color.y, color.z, color.w);

  glPointSize(size);
  glDrawArrays(GL_POINTS, 0, 1);
}

float* circle_generate_verts(float radius, int num_verts) {
  float* verts = malloc(sizeof(float) * num_verts * 2);
  double angle = 360.f/num_verts;
  double deg_to_rad = M_PI/180;
  for (int i = 0; i < num_verts; i++) {
    float current_angle = (float) (i * angle * deg_to_rad);
    vec2 pos = vec2_create((float) (radius * cos(current_angle)), (float) (radius * sin(current_angle)));
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
void draw_circle(GLuint shader_program, GLuint vao, vec2 pos, float scale, vec4 color, int num_elements) {
  pos = world_to_screen(pos);
  color = normalize_rgba(color);

  glBindVertexArray(vao);
  glUseProgram(shader_program);
 
  set_uniform_1f(shader_program, "rotation", 0);
  set_uniform_2f(shader_program, "translation", pos.x, pos.y);
  set_uniform_2f(shader_program, "scale", scale, scale);
  set_uniform_4f(shader_program, "color", color.x, color.y, color.z, color.w);

  glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (void*) 0); 
}

void draw_line(GLuint shader_program, GLuint vao, float x1, float y1, float x2, float y2, vec4 color) {
  vec2 v1 = world_to_screen(vec2_create(x1, y1));
  vec2 v2 = world_to_screen(vec2_create(x2, y2));
  vec2 pos = vec2_scale(vec2_add(v1, v2), 0.5);

  color = normalize_rgba(color);

  float scale = vec2_mag(vec2_sub(v2, v1));
  float rotation = -atan((v2.y-v1.y)/(v2.x-v1.x));

  glBindVertexArray(vao);
  glUseProgram(shader_program);

  set_uniform_1f(shader_program, "rotation", rotation);
  set_uniform_2f(shader_program, "translation", pos.x, pos.y);
  set_uniform_2f(shader_program, "scale", scale, 1);
  set_uniform_4f(shader_program, "color", color.x, color.y, color.z, color.w);

  glDrawArrays(GL_LINES, 0, 2);
}
