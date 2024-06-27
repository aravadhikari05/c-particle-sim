#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include <glad/gl.h>

GLuint bo_create_and_store(GLenum type, size_t size, void* data, bool dynamic);

GLuint vao_create_and_bind(void);

void vao_attrib(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
