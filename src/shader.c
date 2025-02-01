#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

int shader_from_file(const char *filename, char* buff, int buff_size) {
  //using unbuffered functions

  int fd = open(filename, O_RDONLY, 0);
  if (fd == -1) {
    const char error[] = "Could not open file\n";
    write(STDERR_FILENO, error, strlen(error));
    return -1;
  }

  int bytes_read = 0;
  int total_bytes_read = 0;
  do {
    bytes_read = read(fd, buff + total_bytes_read, buff_size - total_bytes_read);
    if (bytes_read < 0) {
      close(fd);
      const char error[] = "Could not read file\n";
      write(STDERR_FILENO, error, strlen(error));
      return -1;
    } 
    total_bytes_read += bytes_read;
  } while (bytes_read > 0); 

  if (total_bytes_read >= buff_size) {
    close(fd);
    const char error[] = "File size bigger than string size\n";
    write(STDERR_FILENO, error, strlen(error));
    return -1;
  }
  buff[total_bytes_read] = '\0'; 
  return 0;
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

void set_uniform_1f(GLuint shader_program, char* name, float x) {
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform1f(uniform_location, x);
}

void set_uniform_2f(GLuint shader_program, char* name, float x, float y) {
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform2f(uniform_location, x, y);
}

void set_uniform_3f(GLuint shader_program, char* name, float x, float y, float z) {
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform3f(uniform_location, x, y, z);
}

void set_uniform_4f(GLuint shader_program, char* name, float x, float y, float z, float w) {
  GLint uniform_location = glGetUniformLocation(shader_program, name);
  glUniform4f(uniform_location, x, y, z, w);
}

void reset_shader(GLuint shader_program) {
    set_uniform_2f(shader_program, "translation", 0, 0);
    set_uniform_2f(shader_program, "scale", 1, 1);
    set_uniform_4f(shader_program, "color", 1, 1, 1, 1);
    set_uniform_1f(shader_program, "rotation", 0);
}
