#include "mt_sort.h"


// Currently doesn't handle exceedingly large individual ints (overflow)


// comparison function for quicksort
int _comp(const void* a, const void* b) {
  return ( *(int*)a - *(int*)b);
}


// helper to print array data
void _printData(char* d) {
  int i;
  printf("\n");
  for (i = 0; i < size; i++) {
    if (strcmp(d, "unsorted") == 0) {
      printf("%d\n", data[i]);
    } else {
      printf("%d\n", sorted[i]);
    }
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
    printf("Input file could not be opened.\n");
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


// Merge the data into new, sorted array using merge function
// from mergesort algorithm.
void* merge() {

  sorted = malloc(size * sizeof(int));
  int a1 = 0;
  int a2 = size/2;
  // merge until one of the arrays is finished
  while (a1 < size/2 && a2 < size) {
    if (data[a1] < data[a2]) {
      sorted[done] = data[a1];
      a1++;
    } else {
      sorted[done] = data[a2];
      a2++;
    }
    done++;
  }
  // finish first array
  while (a1 < size/2) {
    sorted[done] = data[a1];
    a1++;
    done++;
  }
  // finish second array
  while (a2 < size) {
    sorted[done] = data[a2];
    a2++;
    done++;
  }
  return NULL;
}


// quicksorts (in-place) a portion of the global int array
void* quicksort(void* args) {
  struct quicksort_args* a = (struct quicksort_args*) args;
  // build-in C quicksort
  qsort(data+a->l, (a->r - a->l), sizeof(int), _comp);
  return NULL;
}



// Read the content from shared memory object, write to local file.
int writeConcurrently(char* filename) {
  FILE* file;
  // open file for writing
  file = fopen(filename, "w");
  if (file == NULL) {
    printf("Output file could not be opened.\n");
    return 1;
  }
  // write sorted data as soon as it is sorted
  int numWritten = 0;
  while (numWritten < size) {
    while (numWritten < done) {
      // write everything that has currently been sorted
      fprintf(file, "%d ", sorted[numWritten]);
      numWritten++;
    }
  }
  fclose(file);
  return 0;
}


int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("> Invalid usage. Correct usage: ./mt_sort input_fp.txt output_fp.txt");
    return 1;
  }
  // read input data
  readInputData(argv[1]);
  // 2 threads to each sort 1/2 of the data concurrently
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

  // start timer
  clock_t t;
  t = clock();

  // merge data
  pthread_t thr3;
  pthread_create(&thr3, NULL, &merge, NULL);
  // pthread_join(thr3, NULL);

  // write data as it is sorted
  writeConcurrently(argv[2]);

  // end timer
  t = clock() - t;
  double elapsed = ((double)t)/CLOCKS_PER_SEC;
  printf("Took %f seconds to merge and write.\n", elapsed);
}


