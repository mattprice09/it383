#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *file;
	char *filename = "test.txt";

	/* From page 31 of intro to C programming */
	file = fopen(filename, "w");
	if (file == NULL) {
		fprintf(stderr, "File %s could not be opened\n", filename);
		exit(1);
	}

	fprintf(file, "Hello World!\n");

	fclose(file);
}
