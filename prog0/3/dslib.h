#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/* Stack struct using an array of dynamically allocated size */
struct stack {
  
  /* store the stack's capacity given at instantation, for ease of iteration */
  int capacity;
  /* the amount of elements stored in the array */
  int size;
  int *data;
};
typedef struct stack stack;

/* method definitions for stack */
void stack_deallocate(stack *s);
void stack_init(stack* s, int capacity);
int stack_pop(stack *s);
void stack_push(stack *s, int e);
int stack_size(stack *s);

#endif
