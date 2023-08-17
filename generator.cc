#include "generator.h"

/* Generate N pseudo-random uint32_t’s */
void generator(uint32_t *arr, int seed, int mpi_rank, long N) {
    std::mt19937 generator(seed + mpi_rank);
    std::uniform_int_distribution<uint32_t> distribution(0, std::numeric_limits<uint32_t>::max());
    
    for (long i = 0; i < N; i++) {
        arr[i] = distribution(generator);
    }
}