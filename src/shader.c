#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

#define SHADER 1
#define PROGRAM 2  

static void log_error(GLuint obj, int type) {
  int  success;
  char infoLog[512];
  
  switch (type) {
    case 1:
      glGetShaderiv(obj, GL_COMPILE_STATUS, &success);  
      if (!success) {
        glGetShaderInfoLog(obj, 512, NULL, infoLog);
        printf("SHADER COMPILATION FAILED: %s\n", infoLog);
        exit(-1);
      }
      break;
    case 2:
      glGetProgramiv(obj, GL_COMPILE_STATUS, &success);  
      if (!success) {
        glGetProgramInfoLog(obj, 512, NULL, infoLog);
        printf("SHADER LINKING FAILED: %s\n", infoLog);
        exit(-1);
      }
      break;
    default:
      printf("AN ERROR HAS OCCURED\n");
      exit(-1);
  }
}

GLuint shader_program_create(const char* vertex_shader_source, const char* fragment_shader_source) {
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  log_error(vertex_shader, SHADER);
  
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  log_error(fragment_shader, SHADER);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  log_error(shader_program, PROGRAM);
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader); 

  return shader_program;
}

void set_uniform_1f(GLuint shader_program, char* name, float x, bool is_used) {
  if (is_used) {
    glUseProgram(shader_program);
  }
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform1f(uniform_location, x);
}

void set_uniform_2f(GLuint shader_program, char* name, float x, float y, bool is_used) {
  if (is_used) {
    glUseProgram(shader_program);
  }
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform2f(uniform_location, x, y);
}

void set_uniform_3f(GLuint shader_program, char* name, float x, float y, float z, bool is_used) {
  if (is_used) {
    glUseProgram(shader_program);
  }
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform3f(uniform_location, x, y, z);
}

void set_uniform_4f(GLuint shader_program, char* name, float x, float y, float z, float w, bool is_used) {
  if (is_used) {
    glUseProgram(shader_program);
  }
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform4f(uniform_location, x, y, z, w);
}
