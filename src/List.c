#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "List.h"


List* list_create(void) {
  List* L = malloc(sizeof(List));
  if (!L) {
    printf("list_create: could not malloc list\n");
  }
  L->data = NULL;
  L->cap = 0;
  L->length = 0;
  return L;
}

void list_delete(List* L) {
  if (!L) return;
  if (L->data) {
      free(L->data);
  }
  free(L); 
}

void list_clear(List* L) {
  if (!L) return; 
  L->length = 0;
}

void list_keep(List* L, int length) {
  if (!L) return;
  if (length > L->length) {
    printf("list_keep: provided length is greater than or equal to list length\n");
    return;
  }
  L->length = length;
}

int list_length(List*L) {
  if (!L) return -1; 
  return L->length;
}
 
int list_resize(List* L, int n) {
  if (!L) return -1; 

  if (n == 0) {
    list_clear(L);
    return 0;
  }

  LIST_TYPE* new_data = realloc(L->data, sizeof(LIST_TYPE) * n);
  if (!new_data) {
    printf("list_resize: realloc failed\n");
    return -1;
  }

  L->data = new_data;
  L->cap = n;
  return L->cap;
  
}

LIST_TYPE list_get(List* L, int index) {
  if (!L) return NULL;
  if (index >= L->length || index < 0) {
    printf("list_get: index out of bounds\n");
    return NULL;
  }
  return L->data[index];
}

void list_set(List* L, int index, LIST_TYPE val) {
  if (!L) return;
  if (index >= L->length || index < 0) {
    printf("list_set: index out of bounds\n");
    return;
  }
  L->data[L->length++] = val;
}

int list_push(List* L, LIST_TYPE val) {
  if (!L) return -1; 

  if (L->length >= L->cap) {
    int new_cap = L->cap == 0 ? 16 : L->cap * 2;
    if (list_resize(L, new_cap) == -1) {
      printf("list_push: could not resize list\n");
      return -1;
    }
  }
  L->data[L->length++] = val;
  return L->length - 1;

}

LIST_TYPE list_pop(List *L) {
  if (!L) return NULL;
  if(L->length <= 0) {
    printf("list_pop: list has 0 values\n");
    return NULL;
  }
  return L->data[--L->length];
}

/*
void list_insert(List *L, int val) {
  if (L->free_element != -1) {
    int index = L->free_element;
    L->free_element = L->data[index];
    L->data[index] = val;
  } else {
    list_push(L, val);
  }
}

int list_erase(List*L, int index) {
  L->data[index] = L->free_element; 
  L->free_element = index;
}
*/

