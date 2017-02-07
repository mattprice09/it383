#include "dslib.h"

/* Helper for testing */
void assertEquals(int a, int b, int lineNum) {
  if (a != b) {
    printf("> ERROR: Invalid assertion on line %d. Expected %d, got %d.\n", lineNum, a, b);
    exit(0);
  }
}

/* Run a test suite on the stack struct */
int main() {
  stack mystack;
  
  /* Test initialization function */
  stack_init(&mystack, 5);

  /* test stack capacity and size */
  assertEquals(5, mystack.capacity, __LINE__);
  assertEquals(0, mystack.size, __LINE__);  

  /* push dummy values onto stack */
  /* dummy values: [2, 4, 6, 8, 10] */
  int i;
  for (i = 0; i < mystack.capacity; i++) {
    stack_push(&mystack, (i+1)*2);
  }

  /* test stack capacity and size */
  assertEquals(5, mystack.capacity, __LINE__);
  assertEquals(5, mystack.size, __LINE__);

  /* test stack_size() function */
  int ss = stack_size(&mystack);
  assertEquals(5, ss, __LINE__);    
  
  /* test elements in correct order */
  assertEquals(2, mystack.data[0], __LINE__);
  assertEquals(8, mystack.data[3], __LINE__);  

  /* test stack_push on full stack doesn't change stack */
  stack_push(&mystack, 99);
  assertEquals(10, mystack.data[4], __LINE__);
  assertEquals(5, mystack.size, __LINE__);

  /* test stack_pop works */
  int val = stack_pop(&mystack);
  assertEquals(10, val, __LINE__);
  val = stack_pop(&mystack);
  assertEquals(8, val, __LINE__);
  
  /* test stack_pop on empty stack */
  for (i = 0; i < mystack.capacity+1; i++) {
    val = stack_pop(&mystack);
  }
  val = stack_pop(&mystack);
  assertEquals(-1, val, __LINE__);

  /* deallocate dynamic memory from malloc */
  stack_deallocate(&mystack);
  
  printf("> Successfully passed all tests!\n");
}
