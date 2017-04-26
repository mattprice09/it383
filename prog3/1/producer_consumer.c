#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <pthread.h>

#include "buffer.h"


/* the buffer */
buffer_item buffer[BUFFER_SIZE];
mutex_t locks[BUFFER_SIZE];
sem_t sem;
int in;
int out;
int count;

int insert_item(buffer_item item) {

  sem_wait(&sem);
  while (count == BUFFER_SIZE) {
    // wait for consumer to clear room in buffer
  }
  mutex_lock(&locks[in]);
  // critical section
  buffer[in] = item;
  // keep track of the correct mutex for when we unlock
  int curr = in;
  in = (in + 1) % BUFFER_SIZE;
  count++;
  // exit critical section
  mutex_unlock(&locks[curr]);
  sem_post(&sem);
}

/**
* 1) Get semaphore
* 2) Wait for producer to add item to buffer
* 3) Acquire lock for the item
* 4) Critical section
* 5) Release mutex/semaphore
*/
int remove_item(buffer_item *item) {

  sem_wait(&sem);
  while (count == 0) {
    // wait for producer to write data
  }
  mutex_lock(&locks[out]);
  // critical section
  buffer_item item = buffer[out];
  count--;
  // keep track of the correct mutex for when we unlock
  int curr = out;
  out = (out + 1) % BUFFER_SIZE;
  // exit critical section
  mutex_unlock(&locks[curr]);
  sem_post(&sem);

}


void* consumer(void* args) {
  
}


// initialize global variables
void initialize() {
  in = out = count = 0;
  // initialize semaphore
  sem_init(&sem, 0, BUFFER_SIZE);
  // initialize mutex locks
  for (int i = 0; i < BUFFER_SIZE; i++) {
    mutex_init(&locks[i], USYNC_THREAD, NULL);
    // buffer[i] = NULL;
  }
}


void* producer(void* args) {

}


/**
* Read input file that is expected to contain one word per line
*/
char** readWords(char* filename) {
  // open file
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Input file could not be opened.\n");
    exit(1);
  }
  // read file line by line (1 word per line)
  int size = 0;
  char** data = malloc(size * sizeof(char));
  char* word = malloc(100 * sizeof(char));
  while(fscanf(file, "%s", word) == 1) {
    data = (char**) realloc(data, (size+1) * sizeof(char*));
    data[size] = word;
    size++;
  }
  fclose(file);
  return data;
}


int main(int argc, char *argv[]) {

  // read input from file
  char** words = readWords("wordsEn.txt");

  initialize();



  sem_destroy(&sem);

}
