#include <stdio.h>
#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "shader.h"

#define WINDOW_LENGTH 640
#define WINDOW_HEIGHT 640

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
  
  GLFWwindow* window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_HEIGHT, "phoenix", NULL, NULL);
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

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3 
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  
 
  glBindVertexArray(VAO);
  float rot = 0.0f; 
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT);

    rot += 0.01f;

    glBindVertexArray(VAO);
    glUseProgram(shader_program);
    set_uniform_2f(shader_program, "scale", 0.5f, 0.5f);
    set_uniform_1f(shader_program, "rotation", rot);
    set_uniform_2f(shader_program, "translation", 0.25f, 0.25f);
    set_uniform_4f(shader_program, "color", 1.0f, 0.5f, 0.25f, 1.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
