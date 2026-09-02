#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define CACHE_SIZE 140000000

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt);

uint16_t cache[CACHE_SIZE];

int main(void) {
    cache[1] = 0;

    clock_t start_time = clock();

    uint64_t curr_stop_time, longest_stop_time = 1, n = 2, longest_n = 1; 
    uint64_t max_altitude = 0;

    while (1) {
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
    if ((n & 1) == 0) {
        uint64_t st = 1 + cache[n >> 1];
        cache[n] = st;
        return st;
    }

    uint64_t stop_time = 0, n_new = n;
    uint64_t local_peak = *peak_alt;

    while (n_new >= n) {
        uint64_t elevated = 3 * n_new + 1;
        if (elevated > local_peak) {
            local_peak = elevated;
        }

        // count all factors of 2
        int zeros = __builtin_ctzll(elevated);
        n_new = elevated >> zeros;
        stop_time += 1 + zeros;
    }

    stop_time += cache[n_new];
    cache[n] = stop_time;

    *peak_alt = local_peak;
    return stop_time;
}
