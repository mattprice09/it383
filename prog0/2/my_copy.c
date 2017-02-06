#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("> ERROR: Arguments required: ./my_copy <sourceFilePath> <destinationFilePath>\n");    
  }

  /* Retrieve command-line args */
  char *infile = argv[1];
  char *outfile = argv[2];

  /* Open file for reading */
  int f = open(infile, O_RDONLY);
  if (f < 0) {
    printf("> ERROR opening file %s. Please make sure the input file exists.", infile);
    return 1;
  }

  /* Open file for writing */
  int outf = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 0666);
  if (outf < 0) {    
    printf("> ERROR opening file %s for writing. Please try again.", outfile);
    return 1;
  }

  /* Hard-code a break when it's the end of file because no simply way to reassign initialized char array */ 
  char buffer[100];
  int readSize = read(f, buffer, sizeof(buffer));
  while (readSize > 0) {

    /* write chunk */
    write(outf, buffer, readSize);

    /* read next chunk */
    readSize = read(f, buffer, sizeof(buffer));    
  }   
 
  /* Close files and exit */ 
  close(f);
  close(outf);
  
  return 0;
}
