#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "buffer.h"


// data objects
char** words;
int numWords;
buffer_item buffer[BUFFER_SIZE];

// lock objects
pthread_mutex_t stdout_mutex;
sem_t empty;
sem_t full;
sem_t mutex;

// counters
int in;
int out;


/**
* 1) Wait for lock to signal that there is available buffer space
* 2) Get mutex to write
* 3) Critical section
* 4) Release internal mutex
* 5) Signal that there is data to be read for consumer
*/
int insert_item(buffer_item item) {

  // acquire locks
  sem_wait(&empty);
  sem_wait(&mutex);

  // critical section
  buffer[in] = malloc(strlen(item)*sizeof(buffer_item));
  strcpy(buffer[in], item);
  in = (in + 1) % BUFFER_SIZE;

  // exit critical section
  sem_post(&mutex);
  sem_post(&full);
}


/**
* 1) Wait for lock to signal that there is data to consumed
* 2) Get mutex to read specific buffer slot
* 3) Critical section
* 4) Release internal mutex
* 5) Signal that there is now available buffer space for producer
*/
int remove_item(buffer_item *item) {

  // acquire locks
  sem_wait(&full);
  sem_wait(&mutex);
  
  // critical section
  *item = malloc(strlen(buffer[out])*sizeof(buffer_item));
  strcpy(*item, buffer[out]);
  if (item) {
    out = (out + 1) % BUFFER_SIZE;
  }

  // exit critical section
  sem_post(&mutex);
  sem_post(&empty);

  if (item != NULL) {
    return 0;
  } else {
    return 1;
  }
}


/**
* Read/remove an item from the buffer
*/
void* consumer(void* args) {
  buffer_item item;

  pthread_t tid = pthread_self();

  int r = rand() % 10;
  while (true) {
    sleep(r);

    // perform read
    if (remove_item(&item) == 0) {
      // write safely to stdout
      pthread_mutex_lock(&stdout_mutex);
      printf("[Consumer thread ID: %d] removed an item (word: %s) from the buffer\n", tid, item);
      pthread_mutex_unlock(&stdout_mutex);
    }
    r = rand() % 10;
  }
}


// initialize global variables
void initialize() {
  in = out = 0;
  // initialize lock objects
  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&stdout_mutex, NULL);
  srand(time(NULL));
}


/**
* Produce an item to the buffer
*/
void* producer(void* args) {

  pthread_t tid = pthread_self();

  int r = rand() % 10;
  while (true) {
    sleep(r);
    r = rand() % (numWords-1);

    // perform write
    insert_item(words[r]);

    // write safely to stdout
    pthread_mutex_lock(&stdout_mutex);
    printf("[Producer thread ID: %d] inserted an item (word: %s) to the buffer\n", tid, words[r]);
    pthread_mutex_unlock(&stdout_mutex);
    r = rand() % 10;
  }
}


/**
* Read input file that is expected to contain one word per line
*/
void readWords(char* filename) {
  // open file
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Input file could not be opened.\n");
    exit(1);
  }
  // read file line by line (1 word per line)
  numWords = 0;
  words = (char**) malloc(numWords * sizeof(char*));
  char word[100];
  while(fscanf(file, "%s", word) == 1) {
    words = (char**) realloc(words, (numWords+1) * sizeof(char*));
    words[numWords] = malloc(strlen(word) * sizeof(char));
    strcpy(words[numWords], word);
    numWords++;
  }
  fclose(file);
}


int main(int argc, char *argv[]) {

  // get user input
  if (argc < 4) {
    printf("> ERROR: Correct usage: `./producer_consumer MAX_TIME N_PRODUCERS N_CONSUMERS");
    return 1;
  }
  int TIME_LIMIT = atoi(argv[1]);
  int NUM_PRODUCERS = atoi(argv[2]);
  int NUM_CONSUMERS = atoi(argv[3]);

  // read input from file
  readWords("wordsEn.txt");

  // initialize
  initialize();
  pthread_t producers[NUM_PRODUCERS];
  pthread_t consumers[NUM_CONSUMERS];

  // create/run producer threads
  for (int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_create(&producers[i], NULL, producer, NULL);
  }
  // create/run consumer threads
  for (int i = 0; i < NUM_CONSUMERS; i++) {
    pthread_create(&consumers[i], NULL, consumer, NULL);
  }

  // wait, let program run for the specified time
  sleep(TIME_LIMIT);

  // signal end producer threads
  for (int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_cancel(producers[i]);
  }
  // signal end consumer threads
  for (int i = 0; i < NUM_CONSUMERS; i++) {
    pthread_cancel(consumers[i]);
  }

  // clean up mutexes/semaphores
  sem_destroy(&mutex);
  sem_destroy(&full);
  sem_destroy(&empty);
  pthread_mutex_destroy(&stdout_mutex);

}
