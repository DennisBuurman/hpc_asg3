#ifndef MERGE_H
#define MERGE_H

uint32_t *merge_receive(uint32_t *my_sorted, int my_rank, int sender_rank, long &my_elements);

void merge_send(uint32_t *my_sorted, int my_rank, int dst_rank, long my_elements);

#endif