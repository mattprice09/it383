#include "mt_sort.h"


// Currently doesn't handle exceedingly large individual ints (overflow)


// helper to print array data
void printIntArr() {
  int i;
  printf("\n");
  for (i = 0; i < size; i++) {
    printf("%d\n", data[i]);
  }
  printf("\n");
}


// Read integer array from file containing numbers which are guaranteed
// to be separated by whitespace or newline characters.
// 
void readInputData(char* filename) {
  // open file
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("File could not be opened.\n");
    exit(1);
  }

  // read file number by number
  size = 0;
  data = malloc(size * sizeof(int));
  int num;
  while(fscanf(file, "%d", &num) == 1) {
    // add new number
    data = (int*) realloc(data, (size+1) * sizeof(int*));
    data[size] = num;
    size++;
  }

  fclose(file);
}


// comparison function for quicksort
int comp(const void* a, const void* b) {
  return ( *(int*)a - *(int*)b);
}


// quicksorts (in-place) a portion of the global int array
void* quicksort(void* args) {
  struct quicksort_args* a = (struct quicksort_args*) args;
  printf("l: %d, r: %d\n", data[a->l], data[a->r]);
  // build-in C quicksort
  qsort(data+a->l, (a->r - a->l), sizeof(int), comp);
  return NULL;
}


int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("> Invalid usage. Correct usage: ./mt_sort input_fp.txt output_fp.txt");
    return 1;
  }
  // read input data
  readInputData(argv[1]);
  // create thread IDs
  pthread_t thr1;
  pthread_t thr2;
  // set indeces of list for each thread to operate on
  struct quicksort_args thr1_args;
  struct quicksort_args thr2_args;
  thr1_args.l = 0;
  thr1_args.r = size/2;
  thr2_args.l = size/2;
  thr2_args.r = size;
  // begin running threads
  pthread_create(&thr1, NULL, &quicksort, &thr1_args);
  pthread_create(&thr2, NULL, &quicksort, &thr2_args);
  // wait for threads to finish
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);

  printIntArr();
}


