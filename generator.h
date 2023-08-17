#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <random>

void generator(uint32_t *arr, int seed, int mpi_rank, long N); /* Generate N pseudo-random uint32_t’s */

#endif