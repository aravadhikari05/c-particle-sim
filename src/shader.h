#pragma once

#include <stdbool.h>
#include <glad/gl.h>

GLuint shader_program_create(const char* vertex_shader_source, const char* fragment_shader_source);   

void set_uniform_1f(GLuint shader_program, char* name, float x, bool is_used);

void set_uniform_2f(GLuint shader_program, char* name, float x, float y, bool is_used);

void set_uniform_3f(GLuint shader_program, char* name, float x, float y, float z, bool is_used);

void set_uniform_4f(GLuint shader_program, char* name, float x, float y, float z, float w, bool is_used);
