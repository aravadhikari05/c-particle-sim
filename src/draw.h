#pragma once
#include <glad/gl.h>
#include "math.h"

extern const int WIN_W;
extern const int WIN_H;


struct vec2 world_to_screen(struct vec2 a); 

void draw_point(GLuint shader_program, GLuint VAO, int x0, int y0, struct vec4 color, float point_size);

float* circle_generate_verts(float radius, int num_verts);

unsigned int* circle_generate_indices(int num_verts); 

void draw_circle(GLuint shader_program, GLuint VAO, int x0, int y0, int num_indices, struct vec4 color);
