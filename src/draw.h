#pragma once
#include <glad/gl.h>
#include <stdbool.h>
#include <stdlib.h>

#include "math.h"

extern const int WIN_W;
extern const int WIN_H;

GLuint bo_create_and_store(GLenum type, size_t size, void* data, bool dynamic);

GLuint vao_create_and_bind(void);

void vao_attrib(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

vec2 world_to_screen(vec2 a); 

void draw_point(GLuint shader_program, GLuint vao, int x0, int y0, vec4 color, float point_size);

void draw_line(GLuint shader_program, GLuint vao, float x1, float y1, float x2, float y2, vec4 color);

float* circle_generate_verts(float radius, int num_verts);

unsigned int* circle_generate_indices(int num_verts); 

void draw_circle(GLuint shader_program, GLuint vao, vec2 pos, float scale, vec4 color, int num_elements);

void reset_shader(GLuint shader_program);

