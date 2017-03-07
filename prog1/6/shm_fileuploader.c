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
  struct filesharing_struct* upload;
  upload->flag = false;
  // get file size
  fseek(file, 0, SEEK_END);
  int fsize = (int)ftell(file);
  fseek(file, 0, SEEK_SET);
  // handle files that are too large
  if (fsize > SIZE) {
    printf("\nOops! Maximum file size: %iMB", SIZE);
    free(filename);
    exit(1);
  }
  // read file data
  fgets(upload->content, fsize, file);
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
  ftruncate(shm_fd, SIZE);
  return mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
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
  // free dynamically allocated memory
  free(filename);
}