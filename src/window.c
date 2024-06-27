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


int SCREEN_WIDTH = 400;
int SCREEN_HEIGHT = 400;

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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "phoenix", NULL, NULL);
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

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3 
  };

  unsigned int pointVBO, pointVAO;
  unsigned int lineVBO, lineVAO;
  unsigned int triVBO, triVAO;
  unsigned int quadVBO, quadEBO, quadVAO;

  unsigned int VBO, VAO, EBO;
  
  VAO = vao_create_and_bind();
  VBO = bo_create_and_store(GL_ARRAY_BUFFER, sizeof(point_vertices), point_vertices, GL_STATIC_DRAW);   
  //EBO = bo_create_and_store(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  vao_attrib(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
 
  float rot = 0.0f; 
  struct vec2 a = world_to_screen(vec2_create(300, 300));
  
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT);

    //rot += 0.01f;

    glBindVertexArray(VAO);
    glUseProgram(shader_program);

    //set_uniform_2f(shader_program, "scale", 0.5f, 0.5f);
    //set_uniform_1f(shader_program, "rotation", rot);
    a = vec2_add(a, vec2_create(-0.001f, 0.005f));

    set_uniform_2f(shader_program, "translation", a.x, a.y);
    set_uniform_4f(shader_program, "color", 1.0f, 0.5f, 0.25f, 1.0f);

    //glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
