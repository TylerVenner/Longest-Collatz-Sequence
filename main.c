#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define CACHE_SIZE 80000000

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt);

uint16_t cache[CACHE_SIZE];

int main(void) {
    clock_t start_time = clock();

    uint64_t curr_stop_time, longest_stop_time = 1, n = 1, longest_n = 1; 
    uint64_t max_altitude = 0;

    while(1) {
        // 131071 is 0x1FFF
        if ((n & 0x1FFFF) == 0) {
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

        n++;
    }

    printf("\nLongest sequence of length: %lu (n = %lu)\n", longest_stop_time, longest_n);
    printf("Total numbers processed: %lu (peak altitude = %le)\n\n", n - 1, (double)max_altitude);

    return 0;
}

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt) {
    uint64_t stop_time = 0, n_new = n;

    while (n_new > 1) {
        if (n_new < CACHE_SIZE && cache[n_new] > 0) {
            stop_time += cache[n_new];
            break;
        }  

        if ((n_new & 1) == 0) {
            n_new = n_new >> 1;
            stop_time++;
        } else {
            n_new = (3 * n_new + 1) >> 1;
            stop_time += 2;
        }

        if (n_new > *peak_alt) {
            *peak_alt = n_new;
        }
    }

    cache[n] = stop_time;

    return stop_time; 
}
