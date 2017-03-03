
#include "mrprice_filesharing.h"


void downloadFile(struct filesharing_struct* download) {
  FILE* file;
  char* dlFileName = "download.txt";

  file = fopen(dlFileName, "w");
  if (file == NULL) {
    printf("File could not be opened.");
    exit(1);
  }

  fprintf(file, "%s", download->content);
  fclose(file);
  printf("Download successful! Download can be accessed at %s", dlFileName);
}

void* openSharedFileForReading() {
  int shm_fd;
  shm_fd = shm_open(SH_FILE, O_RDONLY, 0666);
  return mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
}

int main() {
  struct filesharing_struct* download = openSharedFileForReading();
  // // struct filesharing_struct* download = (struct filesharing_struct*) ptr;
  while (1) {
    if (download->flag) {
      // char* dlContent = malloc(sizeof(char) * download->size);
      // printf("%s", download->content);
      printf("File size: %d bytes\n", download->size);
      break;
    }
  }
  // printf("%d", download->size);
  // if (download->flag) {
  //   // download available
  //   downloadFile(download);
  // } else {
  //   // download not available
  //   printf("There is no download available.");
  // }
  shm_unlink(SH_FILE);

}
