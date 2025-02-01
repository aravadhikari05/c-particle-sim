#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "shader.h"
#include "draw.h"
#include "math.h"
#include "body.h"
#include "game.h"
#include "QuadTree.h"
#include "QuadNode.h"

#define red vec4_create(255,0,0,1)
#define trans_red vec4_create(255,200,200,1)

#define green vec4_create(0,255,0,1)

#define blue vec4_create(0,0,255,1)
  
#define yellow vec4_create(255,255,0,1)
#define trans_yellow vec4_create(255,255,200,1)

#define pink vec4_create(255,0,255,1)

#define cyan vec4_create(0,255,255,1)


const int WIN_W = 900;
const int WIN_H = 900;

typedef enum CursorState {
  STATE_DEFAULT,
  STATE_SPAWNING,
  STATE_MAGNET
} CursorState;

typedef struct State {
  int keys[GLFW_KEY_LAST];
  int mouse_buttons[GLFW_MOUSE_BUTTON_LAST];
  float scroll_y;
  vec2 cursor_pos;
  CursorState cursor_state;

} State;

State state_create(void) {
  State self;
  self.keys[0] = 0;
  self.mouse_buttons[0] = 0;
  self.scroll_y = 0;
  self.cursor_pos = vec2_zero();
  self.cursor_state = STATE_DEFAULT;
  return self;
}

void state_change(State* state, float* cursor_radius, vec4* cursor_color) {
  *cursor_radius = 2.5f;
 
  switch (state->cursor_state) {

    case STATE_DEFAULT:
      if (state->keys[GLFW_KEY_S] == 1) {
        *cursor_color = yellow;
        state->cursor_state = STATE_SPAWNING;
      } else if (state->keys[GLFW_KEY_M] == 1) {
        *cursor_color = red;
        state->cursor_state = STATE_MAGNET;
      }    
    break;

    case STATE_SPAWNING:
      if (state->keys[GLFW_KEY_S] == 1) {
        *cursor_color = green;
        state->cursor_state = STATE_DEFAULT;
      } else if (state->keys[GLFW_KEY_M] == 1) {
        *cursor_color = red;
        state->cursor_state = STATE_MAGNET;
      }
    break;
    
    case STATE_MAGNET:
      if (state->keys[GLFW_KEY_S] == 1) {
        *cursor_color = yellow;
        state->cursor_state = STATE_SPAWNING;
      } else if (state->keys[GLFW_KEY_M] == 1) {
        *cursor_color = green;
        state->cursor_state = STATE_DEFAULT;
      }
    break;

  }

}


void error_callback(int error, const char* description) {
  fprintf(stderr, "Error [%d]: %s\n", error, description);
}

static void size_callback(GLFWwindow* window, int width, int height) {
  (void) window;
  glViewport(0, 0, width, height);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  State* state = glfwGetWindowUserPointer(window);
  state->mouse_buttons[button] = action;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  State* state = glfwGetWindowUserPointer(window);
  state->keys[key] = action;
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  State* state = glfwGetWindowUserPointer(window);
  state->cursor_pos.x = xpos;
  state->cursor_pos.y = ypos;
}


static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  State* state = glfwGetWindowUserPointer(window);
  state->scroll_y = (float) yoffset;
}

void process_input(GLFWwindow *window) {
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

  State temp_state = state_create();
  State* state = &temp_state; 
  glfwSetWindowUserPointer(window, state); 
  glfwSetFramebufferSizeCallback(window, size_callback);   

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSwapInterval(1);

  char vertex_shader_source[4096];
  char fragment_shader_source[4096];

  if (shader_from_file("src/basic.vs", vertex_shader_source, 4096) != 0) {
    glfwTerminate();
    return -1;
  }

  if (shader_from_file("src/basic.fs", fragment_shader_source, 4096) != 0) {
    glfwTerminate();
    return -1;
  }

  GLuint shader_program = shader_program_create(vertex_shader_source, fragment_shader_source); 

  float line_vertices[] = {
    -0.5f, 0.0f,
     0.5f, 0.0f
  };
  float point_vertices[] = {
    0.0f, 0.0f
  };
 
  int num_vertices = 32;
  float size = 1.0f/WIN_W; 

  float* circle_verts = circle_generate_verts(size, num_vertices);
  unsigned int* circle_indices = circle_generate_indices(num_vertices);
  
  unsigned int point_vao, point_vbo, line_vao, line_vbo, circle_vao, circle_vbo, circle_ebo;

  point_vao = vao_create_and_bind();
  point_vbo = bo_create_and_store(GL_ARRAY_BUFFER, sizeof(point_vertices), point_vertices, false);
  vao_attrib(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0); 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  line_vao = vao_create_and_bind();
  line_vbo = bo_create_and_store(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, false);
  vao_attrib(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0); 
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  circle_vao = vao_create_and_bind();
  circle_vbo = bo_create_and_store(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, circle_verts, false);
  circle_ebo = bo_create_and_store(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * (num_vertices-2), circle_indices, true);
  vao_attrib(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

  AABB rect = AABB_create(WIN_W/2, WIN_W/2, WIN_H/2, WIN_H/2);  
  QuadTree* qt = qt_create(8, 7, rect); 


  int len_bodies = 35;
  for (int x = 0; x < len_bodies; x++) {
    for (int y = 0; y < len_bodies; y++) {
      float radius = 2.0f;
      float mass = radius;
      float x_pos = (6*radius+0.4)*x + 5;
      float y_pos = (6*radius+0.4)*y + 5;
      AABB rect = AABB_create(x_pos, y_pos, radius, radius);
      qt_insert(qt, body_create(mass, radius, vec2_create(x_pos, y_pos), vec2_create(x % 2 ? -20 : 20, y % 2 ? -20 : 20)), rect);
    }
  }
 
  //===============LEGACY for benchmark===================
  
 /* body bodies[len_bodies];
  for (int i = 0; i < len_bodies; i++) {
    //float x_pos = (WIN_W / (float) len_bodies * i) + (WIN_W / (float) len_bodies * 0.5f);
    float x_pos = 20 + 0.01 * i;
    float y_pos = 250;
    body bod = body_create(2.5f, 2.5f, vec2_create(x_pos, y_pos), vec2_create(0,i % 2 ? -1 : 1)); 
    bodies[i] = bod;
  }*/
  //=====================END=============================

  state->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] = -1;

  double fps_limit = 1.0/60.0;
  double last_update = 0.0;

  
  float primary_radius = 2.5f;
  vec4 primary_color = vec4_create(0, 255, 0, 1.0);
  
  float body_e = 1.0f;
  float border_e = 1.0f;

  int magnetize = 0;
  vec2 magnet_vec = vec2_zero();

  clock_t start, end;
  double cpu_time_used;
  while (!glfwWindowShouldClose(window)) {
    start = clock();
    double now = glfwGetTime();
    double delta_time = now - last_update;
    glfwPollEvents();
    
    if (delta_time > fps_limit) {
      last_update = now;
      qt_rebuild(qt);
      
      
      //for (int i = 0; i < len_bodies; i++) {
      for (int i = 0; i < qt->num_elements; i++) {
        if (magnetize != 0) {
          //magnet_vec = vec2_scale(vec2_sub(state->cursor_pos, bodies[i].pos), magnetize); 
          magnet_vec = vec2_scale(vec2_sub(state->cursor_pos, qt->elements[i].pos), magnetize); 
        }  else {
          magnet_vec = vec2_zero();
        }
        /*
        body_update(&bodies[i], fps_limit,0, magnet_vec);
        handle_border_collision(&bodies[i], border_e, WIN_W, WIN_H);
        for (int j = i + 1; j < len_bodies; j++) {
          handle_body_collision(&bodies[i], &bodies[j], body_e);
        }
        */
        body_update(&qt->elements[i], fps_limit, 0, magnet_vec); 
        handle_border_collision(&qt->elements[i], border_e, WIN_W, WIN_H);

        //qt_check_element_interactions(qt, handle_body_collision, body_e);  
      }

      state_change(state, &primary_radius, &primary_color);
      switch (state->cursor_state) {
        case STATE_DEFAULT:
        break;

        case STATE_SPAWNING:
          if (state->scroll_y != 0) {
            if (state->keys[GLFW_KEY_LEFT_SHIFT] == 1) {
              //darken color
            } else {
              primary_radius += state->scroll_y * 10;
              if (primary_radius < 2.5f) primary_radius = 2.5f;
            }
          }
          if (state->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == 1) {
            float radius = 2.0f;
            float mass = radius;
            AABB rect = AABB_create(state->cursor_pos.x, state->cursor_pos.y, radius, radius);
            qt_insert(qt, body_create(mass, radius, vec2_create(state->cursor_pos.x, state->cursor_pos.y), vec2_zero()), rect);
            //state->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] = -1;
          }
        break;

        case STATE_MAGNET:
          if (state->scroll_y != 0) {
            if (state->keys[GLFW_KEY_LEFT_SHIFT] == 1) {
              //darken color
            }else {
              primary_radius += state->scroll_y * 10;
              if (primary_radius < 2.5f) primary_radius = 2.5f;
            }
          }
          if (state->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == 1) {
            magnetize = 1;
          } else if (state->mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT] == 1){
            magnetize = -1;
          }
          else {
            magnetize = 0;
          }
        break;

      }
     
      state->scroll_y = 0;
      //draw

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
      glClear(GL_COLOR_BUFFER_BIT);


      draw_circle(shader_program, circle_vao, state->cursor_pos, primary_radius*2, primary_color, (num_vertices - 2) * 3); 

      for (int i = 0; i < list_length(qt->nodes); i++) {
        QuadNode* node = list_get(qt->nodes, i);
        if (node->first_child == -1) continue;
        draw_line(shader_program, line_vao, node->bbox.cx - node->bbox.hx, node->bbox.cy, node->bbox.cx + node->bbox.hx, node->bbox.cy, cyan);
        draw_line(shader_program, line_vao, node->bbox.cx, node->bbox.cy - node->bbox.hy, node->bbox.cx, node->bbox.cy + node->bbox.hy, cyan);
        draw_point(shader_program, point_vao, node->bbox.cx, node->bbox.cy, cyan, 8.0);
      }
        

      //for (int i = 0; i < len_bodies; i++) {
      for (int i = 0; i < qt->num_elements; i++) {
        //draw_circle(shader_program, circle_vao, bodies[i].pos,bodies[i].radius * 2, yellow, (num_vertices - 2) * 3);   
        draw_circle(shader_program, circle_vao,  qt->elements[i].pos, qt->elements[i].radius*2, yellow, (num_vertices - 2) * 3);   
      } 
      glfwSwapBuffers(window); 
      process_input(window);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  
    printf("%e, num_elements: %d, quadnodes: %d\n", cpu_time_used, qt->num_elements, list_length(qt->nodes));

  }
  glfwTerminate();
  return 0;
}
