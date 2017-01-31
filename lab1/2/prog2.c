#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *file;
	char *filename = "test.txt";

	/* open file for writing */
	file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "File %s could not be opened\n", filename);
		exit(1);
	}

	/* loop while reading a line at a time from the file and printing */
	while (1) {
		char buffer[80];
		fgets(buffer, 80, file);

		/* if it's end of file, break loop */
		if (feof(file)) {
			break;
		}

		printf("%s", buffer);
	}

	/* close file */
	fclose(file);
}
