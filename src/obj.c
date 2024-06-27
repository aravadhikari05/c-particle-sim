#include "obj.h"

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
