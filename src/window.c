#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "shader.h"
#include "obj.h"
#include "draw.h"
#include "math.h"
#include "body.h"


const int WIN_W = 500;
const int WIN_H = 500;

const char* vertex_shader_source = "#version 410 core\n"
  "layout (location = 0) in vec2 aPos;\n"
  "out vec4 vertexColor;\n"
  "uniform vec2 scale = vec2(1.0, 1.0);\n"
  "uniform float rotation = 0.0;\n"
  "uniform vec2 translation = vec2(0.0, 0.0);\n"
  "uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);\n"
  "void main() {\n"
  "  mat2 R = mat2(cos(rotation), -sin(rotation),\n"
  "                sin(rotation), cos(rotation));\n"
  "  gl_Position = vec4((R * (scale * aPos)) + translation, 0.0, 1.0);\n"
  "  vertexColor = color;\n"
  "}\0";

const char* fragment_shader_source = "#version 410 core\n"
  "out vec4 FragColor;\n"
  "in vec4 vertexColor;\n"
  "void main() {\n"
  "  FragColor = vertexColor;\n"
  "}\0"; 

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error [%d]: %s\n", error, description);
}

static void size_callback(GLFWwindow* window, int width, int height) {
  (void) window;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(void) {
  if (!glfwInit()) {
    printf("GLFW could not be initialized\n");
    return -1;
  }
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "phoenix", NULL, NULL);
  if (!window) {
    printf("Window could not be created\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  glfwSetFramebufferSizeCallback(window, size_callback);   
  glfwSwapInterval(1);

  GLuint shader_program = shader_program_create(vertex_shader_source, fragment_shader_source); 

  float vertices[] = {
     0.5f,  0.5f,
     0.5f, -0.5f,
    -0.5f, -0.5f,
    -0.5f,  0.5f,
  };

  float point_vertices[] = {
     0.0f,  0.0f
  };

  float line_vertices[] = {
    -0.5f, 0.0f,
     0.5f, 0.0f
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3 
  };
  
  int num_vertices = 32;
  float size = 1.0f/WIN_W; 

  float* circle_verts = circle_generate_verts(size, num_vertices);
  unsigned int* circle_indices = circle_generate_indices(num_vertices);
  
  unsigned int circle_vao, circle_vbo, circle_ebo;
  
  circle_vao = vao_create_and_bind();
  circle_vbo = bo_create_and_store(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, circle_verts, true);
  circle_ebo = bo_create_and_store(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * (num_vertices-2), circle_indices, true);

  vao_attrib(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    
  int len_bodies = 10;
  struct body bodies[len_bodies];
  for (int i = 0; i < len_bodies; i++) {
    float x_pos = (WIN_W / (float) len_bodies * i) + (WIN_W / (float) len_bodies * 0.5f);
    struct body bod = body_create(5.0f, 5.0f, vec2_create(x_pos, 200.0f), vec2_create(0, 1)); 
    bodies[i] = bod;
  }
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT);
   
    for (int i = 0; i < len_bodies; i++) {
      draw_circle(shader_program, circle_vao, bodies[i].position, bodies[i].radius, vec4_create(255, 255, 0, 1.0), (num_vertices - 2) * 3); 
      if (bodies[i].position.y + bodies[i].radius >= WIN_H - 1 || bodies[i].position.y - bodies[i].radius <= 0) {
        bodies[i].velocity.y = -bodies[i].velocity.y;
      }
      bodies[i].position.y += bodies[i].velocity.y;
      
    } 

    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
