#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

	/* Get file name from user */
	printf("Please enter a file name to read from:\n");
	char fname[100];
	scanf("%s", fname);
		
	/* Open file for reading */
	FILE *file = fopen(fname, "r");
	if (!file) {
		fprintf(stderr, "File %s could not be opened.\n", fname);
		return 1;
	}
	
	/* Iterate over the lines of the file */
	int nlines = 0;
	int nwords = 0;
	char *ctok;
	while (1) {
		char buffer[80];
		fgets(buffer, 80, file);
		
		/* end of file */
		if (feof(file)) {
			break;
		}
		
		for (ctok = strtok(buffer, " "); ctok != NULL; ctok = strtok(NULL, " ")) {
			nwords++;
		}
		nlines++;
	}
	fclose(file);

	printf("The file contains %d lines and %d words.\n", nlines, nwords);

	return(0);
}

