#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>


// Globals

// file name for shared memory object
const char* SH_FILE = "mrprice_filesharing";

// struct to be used for sharing upload/download data
struct filesharing_struct {
  bool flag;
  int size;
  // max file size of 10MB
  char content[10485760];
  char ftype[100];
};
