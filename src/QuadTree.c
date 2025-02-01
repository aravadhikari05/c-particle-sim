#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "QuadTree.h"
#include "game.h"
#include "math.h"



static bool AABB_intersect(AABB a, AABB b) {
  if (fabs(a.cx - b.cx) > (a.hx + b.hx)) {
    return false;
  }

  if (fabs(a.cy - b.cy) > (a.hy + b.hy)) {
    return false;
  }

  return true;
}

AABB AABB_create(float cx, float cy, float hx, float hy) {
  AABB self;
  self.cx = cx;
  self.cy = cy;
  self.hx = hx;
  self.hy = hy;
  return self;
}

static void get_bounding_box(body element, AABB* bbox) {
  bbox->cx = element.pos.x;
  bbox->cy = element.pos.y;
  bbox->hx = element.radius;
  bbox->hy = element.radius;
}

QuadNode* qn_create(int max_elements_per_node, int depth, AABB bbox) {
  QuadNode* qn = malloc(sizeof(QuadNode));
  if (!qn) {
    printf("qn_create: failed to malloc QuadNode\n");
    return NULL;
  }

  qn->first_child = -1;
  qn->num_elements = 0;
  qn->node_elements = calloc(16, sizeof(int));
  qn->node_elements_size = 16;
  qn->depth = depth; //set depth

  qn->bbox = bbox;
  return qn;
}

QuadTree* qt_create(int max_elements_per_node, int max_depth, AABB bbox) {
  QuadTree* Q = malloc(sizeof(QuadTree));
  Q->elements = malloc(sizeof(body) * 4096);
  Q->num_elements = 0;
  Q->max_depth = max_depth;
  Q->bbox = bbox;

  List* node_list = list_create();

  QuadNode* qn = qn_create(max_elements_per_node, 0, bbox);

  list_push(node_list, qn);

  Q->nodes = node_list;
  Q->max_elements_per_node = max_elements_per_node;

  return Q;
}

List* find_leaves(QuadTree* Q, AABB bbox) {
  if (!Q) return NULL;
  List* leaves = list_create();
  List* stack = list_create();

  list_push(stack, list_get(Q->nodes, 0));
  while (list_length(stack) > 0) {
    QuadNode* node = list_pop(stack);


    if (node->first_child == -1) { //leaf
      list_push(leaves, node);
    } else {
      for (int i = 0; i < 4; i++) {
        QuadNode* child = (QuadNode*) list_get(Q->nodes, node->first_child + i);

        if (AABB_intersect(bbox, child->bbox)) {
          list_push(stack, child);
        }
      }
    }
  }
  list_delete(stack);
  return leaves;
}

void qt_insert(QuadTree* Q, body element, AABB bbox) {
  Q->elements[Q->num_elements++] = element;
  int element_index = Q->num_elements - 1;

  List* leaves = find_leaves(Q, bbox);
  for (int i = 0; i < list_length(leaves); i++) {
    QuadNode* node = list_get(leaves, i);
    node_insert_element(Q, element_index, node);
  }
  list_delete(leaves); 
}

void qt_rebuild(QuadTree* Q) {
  qt_clear(Q);
  for (int i = 0; i < Q->num_elements; i++) {
    AABB bbox;
    get_bounding_box(Q->elements[i], &bbox); 
    List* leaves = find_leaves(Q, bbox);
    for (int j = 0; j < list_length(leaves); j++) {
      QuadNode* node = list_get(leaves, j);
      node_insert_element(Q, i, node);
    }
    list_delete(leaves); 
  }
}



void node_insert_element(QuadTree* Q, int new_element_index, QuadNode* node) {
  if (!Q) return;
  if (node->first_child != -1) return;
  if (node->num_elements >= Q->max_elements_per_node && node->depth < Q->max_depth) {
      
    qt_subdivide(Q, node);
    AABB bbox;
    for (int i = 0; i < node->num_elements; i++) {
      int element_index = node->node_elements[i];
     
      get_bounding_box(Q->elements[element_index], &bbox);

      for (int j = 0; j < 4; j++) {
        QuadNode* child =  list_get(Q->nodes, node->first_child + j);
        if (AABB_intersect(bbox, child->bbox)) {
          node_insert_element(Q, element_index, child);
        }
      }
    }
    get_bounding_box(Q->elements[new_element_index], &bbox);
    for (int j = 0; j < 4; j++) {
      QuadNode* child = list_get(Q->nodes, node->first_child + j);
      if (AABB_intersect(bbox, child->bbox)) {
        node_insert_element(Q, new_element_index, child);
      }
    }

    free(node->node_elements);
    node->node_elements = NULL;
    node->num_elements = 0;
  } else { 
    if (node->num_elements >= node->node_elements_size) {
      int* new_node_elements = realloc(node->node_elements, sizeof(int) * node->node_elements_size * 2);
      if (!new_node_elements) {
        printf("node_insert_element: realloc failed\n");
        return;
      }
      printf("node_insert_element: realloc success\n");
      node->node_elements = new_node_elements;
      node->node_elements_size = node->node_elements_size * 2;
    }
    node->node_elements[node->num_elements++] = new_element_index;
  }
}

void qt_subdivide(QuadTree *Q, QuadNode* node) {
  float left_cx, top_cy, bot_cy, right_cx, child_hx, child_hy, parent_cx, parent_cy;
  
  parent_cx = node->bbox.cx;
  parent_cy = node->bbox.cy;
  child_hx = node->bbox.hx/2.0f;
  child_hy = node->bbox.hy/2.0f;

  
  left_cx = parent_cx - child_hx;
  top_cy = parent_cy - child_hy;
  right_cx = parent_cx + child_hx;
  bot_cy = parent_cy + child_hy;

  AABB bbox = AABB_create(left_cx, top_cy, child_hx, child_hy);

  QuadNode* top_left = qn_create(Q->max_elements_per_node, node->depth + 1, bbox);
  bbox.cx = right_cx;
  QuadNode* top_right = qn_create(Q->max_elements_per_node, node->depth + 1, bbox);
  bbox.cy = bot_cy;
  QuadNode* bot_right = qn_create(Q->max_elements_per_node, node->depth + 1, bbox);
  bbox.cx = left_cx;
  QuadNode* bot_left = qn_create(Q->max_elements_per_node, node->depth + 1, bbox);

  int first_child_index = list_push(Q->nodes, top_left);
  list_push(Q->nodes, top_right);
  list_push(Q->nodes, bot_left);
  list_push(Q->nodes, bot_right);

  node->first_child = first_child_index; 
}

void qt_check_element_interactions(QuadTree* Q, void (*handle_collision)(body*, body*, float), float e) {
  QuadNode* root = (QuadNode*) list_get(Q->nodes, 0);
  List* leaves = find_leaves(Q, root->bbox);
  for (int leaf_index = 0; leaf_index < list_length(leaves); leaf_index++) {
    QuadNode* leaf = (QuadNode*) list_get(leaves, leaf_index); 
    int* element_indices = leaf->node_elements;
    for (int element_a_index = 0; element_a_index < leaf->num_elements; element_a_index++) {
      for (int element_b_index = element_a_index + 1; element_b_index < leaf->num_elements; element_b_index++) {
        handle_collision(&Q->elements[element_indices[element_a_index]], &Q->elements[element_indices[element_b_index]], e);
      } 
    }
  }
  list_delete(leaves); 
}

void qt_clear(QuadTree* Q) {
  if (Q && Q->nodes) {
    for (int i = 1; i < Q->nodes->length; i++) {
      QuadNode* qn = (QuadNode*) list_get(Q->nodes, i);
      if (qn) {
        if (qn->node_elements) {
          free(qn->node_elements); 
        }
        free(qn);
      }
    }
    list_clear(Q->nodes);
    QuadNode* qn = qn_create(Q->max_elements_per_node, 0, Q->bbox);
    list_push(Q->nodes, qn);
  }
}

void qt_delete(QuadTree* Q) {
  if (Q) {
    free(Q->elements);
    qt_clear(Q);
    list_delete(Q->nodes);
    free(Q);
  }
}

