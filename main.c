#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define MAX_SEED   150000000
#define CACHE_SIZE (MAX_SEED / 2)

uint64_t compute_stop_time(uint64_t n, uint64_t *peak_alt);

// Cache stores ONLY odd numbers: index = n >> 1
// cache[0] is n=1, cache[1] is n=3, cache[2] is n=5, etc.
uint16_t cache[CACHE_SIZE];

int main(void) {
    cache[0] = 0; // Seed n = 1 (1 >> 1 = 0)

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

        if ((n & 1) == 0) {
            // strip all powers of 2
            int zeros = __builtin_ctzll(n);
            uint64_t odd_base = n >> zeros;

            // already in the cache
            curr_stop_time = zeros + cache[odd_base >> 1];
        } else {
            curr_stop_time = compute_stop_time(n, &max_altitude);
            cache[n >> 1] = (uint16_t)curr_stop_time;
        }

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
    uint64_t local_peak = *peak_alt;

    while (n_new >= n) {
        uint64_t elevated = 3 * n_new + 1;
        if (elevated > local_peak) {
            local_peak = elevated;
        }

        int zeros = __builtin_ctzll(elevated);
        n_new = elevated >> zeros;
        stop_time += 1 + zeros;
    }

    stop_time += cache[n_new >> 1];

    *peak_alt = local_peak;
    return stop_time;
}
