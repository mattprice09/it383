
#include "mrprice_filesharing.h"

// Read the content from shared memory object, write to local file.
void downloadFile(struct filesharing_struct* download) {
  FILE* file;
  // open file for writing
  char* dlFileName = "download.txt";
  file = fopen(dlFileName, "w");
  if (file == NULL) {
    printf("File could not be opened.");
    exit(1);
  }
  // write download data to local file
  fprintf(file, "%s", download->content);
  fclose(file);
  printf("\nDownload successful! Download can be accessed at %s\n", dlFileName);
}

// Open shared memory object for reading
void* openSharedFileForReading() {
  int shm_fd;
  shm_fd = shm_open(SH_FILE, O_RDONLY, 0666);

  // handle when there isn't a shared memory object available
  if (shm_fd == -1) {
    printf("\nOops! There is no download available.\n");
    exit(1);
  }
  return mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
}

int main() {
  struct filesharing_struct* download = openSharedFileForReading();
  if (download->flag) {
    // download available
    downloadFile(download);
  } else {
    // download not available
    printf("There is no download available.");
  }
  shm_unlink(SH_FILE);
}
