#include <stdio.h>
#include <time.h>

#define MAX(x, y) ((x) > (y))?(x):(y)

unsigned long compute_length(unsigned long int n);

int main(void) {
	clock_t start_time = clock();

	unsigned long current_length, longest_length = 1, n = 1;

	while(1) {
		double elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		if (elapsed_seconds >= 1.0) {
			break;
		}

		current_length = compute_length(n);
		longest_length = MAX(current_length, longest_length); 
		n++;
	}

	printf("Longest sequence of length: %lu\n", longest_length);

	return 0;
}

unsigned long compute_length(unsigned long n) {
	unsigned long length = 0;

	while (n > 1) {
		if (n % 2 == 0) {
			n = n / 2;
		} else {
			n = 3 * n + 1;
		}
		length++;
	}

	return length; 
}
