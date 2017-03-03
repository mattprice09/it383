#include "mrprice_filesharing.h"


// open shared memory object for writing
void* openSharedFileForWriting() {
  int shm_fd;
  shm_fd = shm_open(SH_FILE, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, SIZE);
  return mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
}

// read file data from local file into filesharing struct
struct filesharing_struct readFileData(char* filename) {
  // open file
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("File could not be opened.");
    exit(1);
  }
  // get file size
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);
  // read file data
  // char *fdata = malloc(fsize + 1);
  // fread(upload.content, fsize, 1, file);
  char fdata[SIZE];
  fread(fdata, fsize, 1, file);
  fclose(file);
  printf("%s", fdata);
  struct filesharing_struct upload;
  upload.flag = false;
  upload.size = (int)fsize;
  // strcpy(upload.content, fdata);
  memcpy(upload.content, fdata, sizeof(fdata));
  // create struct
  return upload;
}

int main() {

  // get file path of file to upload
  char* filename = malloc(100 * sizeof(char));
  printf("Please enter the file path of the file to share: \n> ");
  fgets(filename, 100, stdin);
  // remove newline character
  filename[strlen(filename)-1] = 0;

  struct filesharing_struct data = readFileData(filename);
  
  // create shared memory object
  struct filesharing_struct* upload = openSharedFileForWriting();

  // create shared memory object after getting user file name
  // struct filesharing_struct* upload = (struct filesharing_struct*) ptr;
  // upload->flag = false;
  // upload->size = data.size;
  // strcpy(upload->content, data.content);
  // upload->content = malloc(sizeof(char) * strlen(data.content));
  memcpy(upload, &data, sizeof(struct filesharing_struct)+sizeof(data.content));
  // memcpy(upload->content, &data.content, data.size);
  // upload->flag = true;
  // free(data.content);
  // free(upload->content);
  free(filename);
}