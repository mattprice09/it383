#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


// global variables
int* data;
int size;

// stores args for quicksort threads
struct quicksort_args {
  int l;
  int r;
};
