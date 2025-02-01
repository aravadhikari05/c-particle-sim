#pragma once

#include "QuadNode.h"
#define LIST_TYPE QuadNode*

typedef struct List {
  LIST_TYPE* data;
  int cap;
  int length;
} List;

List* list_create(void);

void list_delete(List* L);

void list_clear(List* L);

void list_keep(List* L, int length);

LIST_TYPE list_get(List* L, int index);

void list_set(List* L, int index, LIST_TYPE val);

int list_length(List* L);

int list_push(List* L, LIST_TYPE val);

LIST_TYPE list_pop(List *L); 
