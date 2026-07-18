#include <stdio.h>
#include <time.h>

unsigned long compute_length(unsigned long int n, unsigned long *peak_alt);

int main(void) {
	clock_t start_time = clock();

	unsigned long current_length, longest_length = 1, n = 1, longest_n, max_altitude = 0;

	while(1) {
		double elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		if (elapsed_seconds >= 1.0) {
			break;
		}

		current_length = compute_length(n, &max_altitude);

		if (current_length > longest_length) {
			longest_n = n;
			longest_length = current_length;
		}
		n++;
	}

	printf("\nLongest sequence of length: %lu (n = %lu)\n", longest_length, longest_n);
	printf("Total numbers processed: %lu (peak altitude = %lu)\n\n", n, max_altitude);

	return 0;
}

unsigned long compute_length(unsigned long n, unsigned long *peak_alt) {
	unsigned long length = 0;

	while (n > 1) {
		if (n % 2 == 0) {
			n = n / 2;
		} else {
			n = 3 * n + 1;
		}

		if (n > *peak_alt) {
			*peak_alt = n;
		}

		length++;
	}

	return length; 
}
