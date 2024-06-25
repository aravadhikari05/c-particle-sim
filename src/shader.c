#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>

#define SHADER 1
#define SHADER_PROGRAM 2  

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

GLuint create_shader_program(char* vertex_shader_source, char* fragment_shader_source) {
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
  log_error(shader_program, SHADER_PROGRAM);
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader); 

  return shader_program;
}
