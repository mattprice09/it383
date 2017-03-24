#include "mrprice_filesharing.h"


// read file data from local file into filesharing struct
struct filesharing_struct* readFileData(char* filename) {
  // open file
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("File could not be opened.");
    exit(1);
  }
  // struct for storing upload data
  struct filesharing_struct* upload = malloc(sizeof(struct filesharing_struct));
  upload->flag = false;
  // get file size
  fseek(file, 0, SEEK_END);
  int fsize = (int)ftell(file);
  fseek(file, 0, SEEK_SET);
  // copy file data
  char buffer[512];
  strcpy(upload->content, "");
  while (fgets(buffer, sizeof(buffer), file)) {
    strcat(upload->content, buffer);
  } 
  fclose(file);
  // set struct's file size and flag
  upload->size = fsize;
  upload->flag = true;
  return upload;
}

// open shared memory object for writing
void* openSharedFileForWriting() {
  int shm_fd;
  shm_fd = shm_open(SH_FILE, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, sizeof(struct filesharing_struct));
  return mmap(0, sizeof(struct filesharing_struct), PROT_WRITE, MAP_SHARED, shm_fd, 0);
}

int main() {
  // get file path of file to upload
  char* filename = malloc(100 * sizeof(char));
  printf("Please enter the file path of the file to share: \n> ");
  fgets(filename, 100, stdin);
  // remove newline character
  filename[strlen(filename)-1] = 0;
  // read upload data to struct
  struct filesharing_struct* data = readFileData(filename);
  // create shared memory object
  struct filesharing_struct* upload = openSharedFileForWriting();
  // write struct data to shared memory
  upload->flag = false;
  upload->size = data->size;
  strcpy(upload->content, data->content);
  upload->flag = true;
  // find where the file extension begins
  int i;
  int lastDot = -1;
  for (i = 0; i < strlen(filename); i++) {
    if (filename[i] == '.') {
      lastDot = i;
    }
  }
  // take note of the upload's file extension
  if (lastDot != -1) {
    strncpy(upload->ftype, filename+lastDot, i-lastDot);
  } else {
    strcpy(upload->ftype, "");
  }
  // free dynamically allocated memory
  free(filename);
}
