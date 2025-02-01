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

