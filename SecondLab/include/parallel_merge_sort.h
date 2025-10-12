#ifndef PARALLEL_MERGE_SORT_H
#define PARALLEL_MERGE_SORT_H

void merge_sort_sequential(int arr[], int temp[], int left, int right);
void merge_sort_parallel(int arr[], int temp[], int left, int right, int num_threads);

#endif
