#pragma once

typedef struct AABB {
  float cx;
  float cy;

  float hx;
  float hy;

} AABB;

typedef struct QuadNode {
  int first_child;

  int* node_elements; //points to a list of elements in the node
  int num_elements;
  int node_elements_size;
  int depth;

  AABB bbox;
} QuadNode;

typedef struct QuadNode {
  int first_child; //points to quad node or element instance
  int num_elements; //number of elements 
  int depth;

}

typdef struct ElementInstance {
  int next; //index of next element instance in leaf
  int element; //index of actual element;
}



