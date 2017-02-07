#include "dslib.h"

/* Deallocate memory used by the stack */
void stack_deallocate(stack *s) {
  free(s->data);
}

/* Initialize the stack */
void stack_init(stack *s, int capacity) {
  s->capacity = capacity;
  s->size = 0;
  s->data = (int*)malloc(capacity*sizeof(int));
}

/* Pop/Return the top element of the stack */
int stack_pop(stack *s) {
  if (s->size == 0) {
    return -1;
  }
  s->size--;
  
  /* get popped value */  
  int popped = s->data[s->size];
  s->data[s->size] = 0;
  return popped;
}

/* Push an element to the stack */
void stack_push(stack *s, int e) {
  if (s->size != s->capacity) {
    s->data[s->size] = e;
    s->size++;
  }
}

/* Get the number of elements currently stored in the stack */
int stack_size(stack *s) {
  return s->size;
}

