#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>


// global variables
int* data;
int size;
int* sorted;
int done;

// stores args for quicksort threads
struct quicksort_args {
  int l;
  int r;
};
