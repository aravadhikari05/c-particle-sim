#include <stdio.h>
#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "shader.h"

#define WINDOW_LENGTH 640
#define WINDOW_HEIGHT 480

char* vertex_shader_source = "#version 410 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

char* fragment_shader_source = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "  FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
  "}\0"; 

char* fragment_shader_source2 = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}\0"; 

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void size_callback(GLFWwindow* window, int width, int height) {
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
  
  GLFWwindow* window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_HEIGHT, "Phoenix", NULL, NULL);
  if (!window) {
    printf("Window could not be created\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  glfwSetFramebufferSizeCallback(window, size_callback);   

  glfwSwapInterval(1);

  GLuint shader_program = create_shader_program(vertex_shader_source, fragment_shader_source); 
  GLuint shader_program2 = create_shader_program(vertex_shader_source, fragment_shader_source2); 

  const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
     -0.25f,  0.0f, 0.0f,
  }; 

  const float vertices2[] = {
    0.5f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
     0.25f,  0.0f, 0.0f
  };

  const float sq_vertices[] = {
       0.0f,  0.0f, 0.0f,  // top right
       0.0f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.0f, 0.0f   // top left
  };

  unsigned int sq_indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int VBOs[2], VAOs[2], EBO;
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAOs[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sq_vertices), sq_vertices, GL_STATIC_DRAW); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sq_indices), sq_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  
 
  glBindVertexArray(VAOs[1]);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW); 
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);   

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(shader_program2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
