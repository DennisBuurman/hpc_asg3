#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <mpi.h>

#include "generator.h"
#include "sort.h"
#include "merge.h"
#include "util.h"


uint32_t *generate_data(int seed, int rank, long size) {
    uint32_t *sorted = (uint32_t *)malloc(size * sizeof(uint32_t));
    generator(sorted, seed, rank, size);

    return sorted;
}

void merge(uint32_t *sorted, long sorted_size, int n_procs, int mpi_rank) {
    uint32_t *new_sorted;
    int midpoint;
    int active_procs = n_procs;
    while (active_procs > 1) {
        midpoint = div_ceil(active_procs, 2);
        if (mpi_rank < midpoint) {
            if (midpoint + mpi_rank < active_procs) {
                //This process will receive and merge with its own data
                new_sorted = merge_receive(sorted, mpi_rank, midpoint+mpi_rank, sorted_size);
                free(sorted);
                sorted = new_sorted;
            }
        } else if (mpi_rank < active_procs) {
            //This process will send (but not perform a merge)
            merge_send(sorted, mpi_rank, mpi_rank-midpoint, sorted_size);
            break;
        }

        active_procs = midpoint;
    }
    
}

//TODO make the formatting not be shit
int main(int argc, char **argv) {

    //Read arguments
    if (argc < 3) {
        fprintf(stderr, "usage: %s <array length> <seed> [quick|bubble]\n", argv[0]);
        return -1;
    }
    long N = atol(argv[1]);
    int seed = atoi(argv[2]);
    SortType sortType = quick;
    if (N < 1)
    {
        fprintf(stderr, "Invalid array size specified.\n");
        return -1;
    }

    //Read sorting type if provided
    if (argc > 3) {
        if (!strcmp("quick", argv[3])) {
            sortType = quick;
        } else if (!strcmp("bubble", argv[3])) {
            sortType = bubble;
        } else {
            fprintf(stderr, "WRN: Unknown sort type provided, using default\n");
        }
    }

    //Initialize MPI
    int mpi_comm_size, mpi_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    //Determine data size of this node, and generate array of required size
    uint32_t *sorted;
    int n_procs = std::min(long(mpi_comm_size), N); //Cannot divide n elements over more than n nodes
    long start_index = std::min(mpi_rank*(N/n_procs), N);
    long end_index = std::min((mpi_rank+1)*(N/n_procs), N); //NB: not inclusive
    //Remaining elements are assigned to the last processor. Not the fairest division,
    //but this only shows when the number of elements is very small.
    if (mpi_rank == n_procs-1) {
        end_index = N;
    }
    long sorted_size = end_index-start_index;
    fprintf(stderr, "Rank %d data size: %ld\n", mpi_rank, sorted_size);
    sorted = generate_data(seed, mpi_rank, sorted_size);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    //Step 2: Have process sort its sequence locally
    sort(sorted, sorted_size, sortType);

    //Step 3: merge until done
    merge(sorted, sorted_size, n_procs, mpi_rank);

    auto end_time = std::chrono::high_resolution_clock::now();

    //Show output on root processor
    if (mpi_rank == 0) {
        //print_array("Output", sorted, sorted_size);
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        fprintf(stdout, "%.20f\n", elapsed_time.count());
    }


    //Cleanup
    free(sorted);
    MPI_Finalize();
    

    return 0;
}
