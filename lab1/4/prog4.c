#include <stdio.h>
#include <stdlib.h>

int main() {

	/* The number of integers to retrieve from user */
	int x = 10;
	
	/* Retrieve input values from user */
	int nums[x];
	int n = x-1;
	while (n >= 0) {
		printf("Please enter a number:\n");

		int x;
		if (scanf("%d", &x) == 0) {
			printf("Invalid integer\n");
		} else {
			nums[n] = x;
			n--;
		}
	}

	/* Print values back to user */
	int i;
	for (i = 0; i < x-1; i++) {
		printf("%d, ", nums[i]);
	}
	printf("%d\n", nums[x-1]);

	/* Write the values to a file */
	FILE *fp;
	char *fname = "userNumbers.txt";
	fp = fopen(fname, "w+");
	for (i = 0; i < x; i++) {
		fprintf(fp, "%d\n", nums[i]);
	}
	fclose(fp);
	
	return(0);
}

