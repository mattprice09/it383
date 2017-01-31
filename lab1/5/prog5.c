#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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
	char *fname = "userNumbers.txt";
	int filedesc = open(fname, O_RDWR | O_CREAT | O_TRUNC);
	if (filedesc < 0) {
		printf("Error opening file %s...\n", fname);
		return 1;
	}
	for (i = 0; i < x; i++) {
		char ichar = nums[i]+'0';
		write(filedesc, &ichar, sizeof(ichar));
	}
	close(filedesc);
	
	return(0);
}

