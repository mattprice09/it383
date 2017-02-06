#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("> ERROR: Arguments required: ./my_copy <sourceFilePath> <destinationFilePath>\n");    
  }

  /* Retrieve command-line args */
  char *infile = argv[1];
  char *outfile = argv[2];

  /* Open file for reading */
  FILE *f = fopen(infile, "r");
  if (!f) {
    printf("> ERROR opening file %s. Please make sure the input file exists.", infile);
    return 1;
  }

  /* Open file for writing */
  FILE *outf = fopen(outfile, "w");
  if (!outf) {    
    printf("> ERROR opening file %s for writing. Please try again.", outfile);
    return 1;
  }

  /* Read file contents*/
  char buffer[100];
  fread(buffer, 1, 200, f);
  
  /* Write to user-specified file */
  fwrite(buffer, sizeof(char), strlen(buffer), outf);
  
  fclose(f);
  fclose(outf);
  return 0;
}
