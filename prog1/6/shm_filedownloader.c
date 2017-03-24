
#include "mrprice_filesharing.h"


char* createDownloadFName(struct filesharing_struct* download) {
  char* dlFileName = malloc(100 * sizeof(char));
  strcpy(dlFileName, "download_");
  char dateStr[100];
  char timeStr[100];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(dateStr, sizeof(dateStr), "%d_%m_%Y_", tm);
  strftime(timeStr, sizeof(timeStr), "%H.%M.%S", tm);
  strcat(dlFileName, dateStr);
  strcat(dlFileName, timeStr);
  strcat(dlFileName, download->ftype);
  return dlFileName;
}

// Read the content from shared memory object, write to local file.
int downloadFile(struct filesharing_struct* download) {
  FILE* file;
  char* dlFileName = createDownloadFName(download);
  // open file for writing
  file = fopen(dlFileName, "wb");
  if (file == NULL) {
    printf("File could not be opened.\n");
    return 1;
  }
  // write download data to local file
  fwrite(download->content, download->size, 1, file);
  fclose(file);
  printf("\nDownload successful! Download can be accessed at %s\n", dlFileName);
  free(dlFileName);
  return 0;
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
  return mmap(0, sizeof(struct filesharing_struct), PROT_READ, MAP_SHARED, shm_fd, 0);
}

int main() {
  struct filesharing_struct* download = openSharedFileForReading();
  if (download->flag) {
    // download available
    int status = downloadFile(download);
  } else {
    // download not available
    printf("There is no download available.");
  }
  shm_unlink(SH_FILE);
}
