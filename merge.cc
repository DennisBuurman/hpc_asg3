#include <cstdint>
#include <climits>
#include <mpi.h>

#include "merge.h"

//NEVER EVER make MPI_BLOCK_SIZE larger than INT_MAX. INT_MAX as value is
//probably most efficient, as amount of messages is kept to a minimum
int MPI_BLOCK_SIZE=INT_MAX;

uint32_t *merge_receive(uint32_t *my_sorted, int my_rank, int sender_rank, long &my_elements) {
    long sender_elements;
    long *sender_elements_buf;
    long new_elements, received_elements;
    uint32_t *sender_sorted;
    uint32_t *new_sorted;
    long new_index, my_index, sender_index;
    int recv_size;

    //First, determine size that will be received
    sender_elements_buf = (long *)malloc(sizeof(long));
    MPI_Recv(
        sender_elements_buf,
        1,
        MPI_LONG,
        sender_rank,
        MPI_ANY_TAG,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE
    );
    sender_elements = *sender_elements_buf;
    delete sender_elements_buf;

    //Receive all elements, over multiple blocks if needed
    sender_sorted = (uint32_t *)malloc(sender_elements*sizeof(uint32_t));
    received_elements = 0;
    while (received_elements < sender_elements) {
        recv_size = int(std::min(sender_elements-received_elements, long(MPI_BLOCK_SIZE)));
        MPI_Recv(
            sender_sorted+received_elements,
            recv_size,
            MPI_UINT32_T,
            sender_rank,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        received_elements += recv_size;
    }

    //Finally, merge the local and received lists
    new_elements = my_elements + sender_elements;
    new_sorted = (uint32_t *)malloc(new_elements * sizeof(uint32_t));
    my_index=0, sender_index=0, new_index=0;
    while (my_index < my_elements && sender_index < sender_elements) {
        if (my_sorted[my_index] < sender_sorted[sender_index]) {
            new_sorted[new_index] = my_sorted[my_index];
            ++my_index;
        } else {
            new_sorted[new_index] = sender_sorted[sender_index];
            ++sender_index;
        }
        ++new_index;
    }
    while (my_index < my_elements) {
        new_sorted[new_index] = my_sorted[my_index];
        ++new_index;
        ++my_index;
    }
    while (sender_index < sender_elements) {
        new_sorted[new_index] = sender_sorted[sender_index];
        ++new_index;
        ++sender_index;
    }

    //Clean up and return
    free(sender_sorted);
    my_elements = new_elements;
    return new_sorted;
}

void merge_send(uint32_t *my_sorted, int my_rank, int dst_rank, long my_elements) {
    long sent_elements;
    int send_size;

    //Send element count of our sorted array to destination
    MPI_Send(
        &my_elements,
        1,
        MPI_LONG,
        dst_rank,
        0,
        MPI_COMM_WORLD
    );


    //Send sorted array to destination, through multiple messages if needed
    sent_elements = 0;
    while (sent_elements < my_elements) {
        send_size = int(std::min(my_elements-sent_elements, long(MPI_BLOCK_SIZE)));
        MPI_Send(
            my_sorted+sent_elements,
            send_size,
            MPI_UINT32_T,
            dst_rank,
            0,
            MPI_COMM_WORLD
        );
        sent_elements += send_size;
    }
}