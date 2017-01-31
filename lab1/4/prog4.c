#include <stdio.h>
#include <stdlib.h>

int main() {

	int nums[10];
	int n = 9;
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
	int i;
	for (i = 0; i < 9; i++) {
		printf("%d", nums[i]);
	}
	
}

