#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


// Globals
// 10MB file size
const int SIZE = 10485760;
// file name for shared memory object
const char* SH_FILE = "mrprice_filesharing";

struct filesharing_struct {
  bool flag;
  int size;
  char* content;
};