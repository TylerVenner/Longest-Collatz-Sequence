#include <stdio.h>
#include <time.h>

#define BIN_SIZE 20
#define NUM_BINS 50
#define CACHE_SIZE 30000000

unsigned long compute_stop_time(unsigned long int n, unsigned long *peak_alt);
void print_histogram(unsigned long histogram[]);

unsigned long cache[CACHE_SIZE];

int main(void) {
	clock_t start_time = clock();

	unsigned long curr_stop_time, longest_stop_time = 1, n = 1, longest_n; 
	unsigned long index, max_altitude = 0, histogram[NUM_BINS] = {0};

	while(1) {
		// only check clock occasionally
		if (n % 10000 == 0) {
			double elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;
			if (elapsed_seconds >= 1.0) {
				break;
			}
		}

		

		curr_stop_time = compute_stop_time(n, &max_altitude);

		if (curr_stop_time > longest_stop_time) {
			longest_n = n;
			longest_stop_time = curr_stop_time;
		}

		index = curr_stop_time / BIN_SIZE;
		if (index >= NUM_BINS) {
			index = NUM_BINS - 1;
		}
		histogram[index]++;

		n++;
	}

	printf("\nLongest sequence of length: %lu (n = %lu)\n", longest_stop_time, longest_n);
	printf("Total numbers processed: %lu (peak altitude = %le)\n\n", n, (double)max_altitude);

	print_histogram(histogram);

	return 0;
}

unsigned long compute_stop_time(unsigned long n, unsigned long *peak_alt) {
	unsigned long stop_time = 0, n_new = n;

	while (n_new > 1) {
		if (n_new < CACHE_SIZE && cache[n_new] > 0) {
			stop_time += cache[n_new];
			break;
		}  

		if (n_new % 2 == 0) {
			n_new = n_new / 2;
		} else {
			n_new = 3 * n_new + 1;
		}

		if (n_new > *peak_alt) {
			*peak_alt = n_new;
		}

		stop_time++;
	}

	cache[n] = stop_time;

	return stop_time; 
}

void print_histogram(unsigned long histogram[]) {
	for (int i = 0; i < NUM_BINS; i++) {
	    unsigned long range_start = i * BIN_SIZE;
	    unsigned long range_end = range_start + BIN_SIZE - 1;
	    
	    if (i == NUM_BINS - 1) {
		printf("%4lu+      | ", range_start);
	    } else {
		printf("%4lu-%-4lu | ", range_start, range_end);
	    }

	    // print one asterisk for every 10,000 items
	    unsigned long stars = histogram[i] / 10000;
	    for (unsigned long s = 0; s < stars; s++) {
		printf("*");
	    }
	    printf(" (%lu)\n", histogram[i]);
	}
}
