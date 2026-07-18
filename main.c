#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define BIN_SIZE 20
#define NUM_BINS 50
#define CACHE_SIZE 40000000
#define HIST_DENSITY 40000

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt);
void print_histogram(uint64_t histogram[]);
void print_telemetry(uint64_t n, uint64_t longest_stop_time, uint64_t longest_n, uint64_t max_altitude, uint64_t theoretical_math_steps, uint64_t histogram[]);

uint64_t cache[CACHE_SIZE];
uint64_t actual_math_steps = 0;

int main(void) {
	clock_t start_time = clock();

	uint64_t curr_stop_time, longest_stop_time = 1, n = 1, longest_n = 1; 
	uint64_t index, max_altitude = 0, histogram[NUM_BINS] = {0};
	uint64_t theoretical_math_steps = 0;

	while(1) {
		// only check clock occasionally
		if (n % 10000 == 0) {
			double elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;
			if (elapsed_seconds >= 1.0) {
				break;
			}
		}

		curr_stop_time = compute_stop_time(n, &max_altitude);
		theoretical_math_steps += curr_stop_time;

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

	print_telemetry(n - 1, longest_stop_time, longest_n, max_altitude, theoretical_math_steps, histogram);
	return 0;
}

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt) {
	uint64_t stop_time = 0, n_new = n;

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
		actual_math_steps++;
	}

	cache[n] = stop_time;

	return stop_time; 
}

void print_histogram(uint64_t histogram[]) {
	printf("Numbers per star (*): %d\n", HIST_DENSITY);
	for (int i = 0; i < NUM_BINS; i++) {
	    uint64_t range_start = i * BIN_SIZE;
	    uint64_t range_end = range_start + BIN_SIZE - 1;
	    
	    if (i == NUM_BINS - 1) {
		printf("%4lu+      | ", range_start);
	    } else {
		printf("%4lu-%-4lu | ", range_start, range_end);
	    }

	    uint64_t stars = histogram[i] / HIST_DENSITY;
	    for (uint64_t s = 0; s < stars; s++) {
		printf("*");
	    }
	    printf(" (%lu)\n", histogram[i]);
	}
}

void print_telemetry(uint64_t n, uint64_t longest_stop_time, uint64_t longest_n, uint64_t max_altitude, uint64_t theoretical_math_steps, uint64_t histogram[]) {
	printf("\nLongest sequence of length: %lu (n = %lu)\n", longest_stop_time, longest_n);
	printf("Total numbers processed: %lu (peak altitude = %le)\n\n", n, (double)max_altitude);

	print_histogram(histogram);

	double math_skipped_pct = 100.0 - ((double)actual_math_steps / theoretical_math_steps * 100.0);

	printf("Theoretical math steps: %lu\n", theoretical_math_steps);
	printf("Actual math steps: %lu\n", actual_math_steps);
	printf("Work bypassed by cache: %.2f%%\n", math_skipped_pct);	
}
