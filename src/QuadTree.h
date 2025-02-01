#pragma once
#include "game.h"
#include "List.h"
#include "QuadNode.h"


typedef struct QuadTree {
  body* elements;
  List* nodes; 

  int max_elements_per_node;
  int num_elements;
  int max_depth;

  AABB bbox;

} QuadTree;

typedef struct nQuadTree {
  body* elements;

  IntList* nodes; 
  IntList* instances;
  
  int cx, cy, hx, hy;

  int max_elements_per_node;
  int num_elements;
  int max_depth;

} nQuadTree;

enum {
  inst_num_fields = 2;
  inst_next = 0;
  inst_element = 1;
  
  node_num_fields = 2;
  node_first_child = 0; 
  node_num_elements = 1; //-1 if it stores nodes

  node_data_fields = 6;
  node_data_index = 0;
  node_data_depth = 1;
  node_data_cx = 2;
  node_data_cy = 3;
  node_data_hx = 4;
  node_data_hy = 5;

}



AABB AABB_create(float cx, float cy, float hx, float hy);

QuadNode* qn_create(int max_elements_per_node, int depth, AABB bbox);
  
QuadTree* qt_create(int max_elements_per_node, int max_depth, AABB bbox);

List* find_leaves(QuadTree* Q, AABB bbox);

void qt_insert(QuadTree* Q, body element, AABB bbox);

void node_insert_element(QuadTree* Q, int element_index, QuadNode* node);

void qt_subdivide(QuadTree *Q, QuadNode* node);

void qt_check_element_interactions(QuadTree* Q, void (*handle_collision)(body*, body*, float), float e);

void qt_rebuild(QuadTree* Q);

void qt_clear(QuadTree* Q);

void qt_delete(QuadTree* Q);

