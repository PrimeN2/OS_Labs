#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parallel_merge_sort.h"
#include "timer.h"

#define ARRAY_SIZE 1000000

void fill_random(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

int main(void) {
    int *original = malloc(ARRAY_SIZE * sizeof(int));
    int *arr = malloc(ARRAY_SIZE * sizeof(int));
    int *temp = malloc(ARRAY_SIZE * sizeof(int));
    Timer timer;

    fill_random(original, ARRAY_SIZE);

    memcpy(arr, original, ARRAY_SIZE * sizeof(int));
    timer_start(&timer);
    merge_sort_sequential(arr, temp, 0, ARRAY_SIZE - 1);
    timer_stop(&timer);
    printf("Sequential time: %.2f ms\n", timer_elapsed_ms(&timer));

    int thread_counts[] = {2, 4, 8};
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    for (int i = 0; i < num_tests; i++) {
        memcpy(arr, original, ARRAY_SIZE * sizeof(int));
        timer_start(&timer);
        merge_sort_parallel(arr, temp, 0, ARRAY_SIZE - 1, thread_counts[i]);
        timer_stop(&timer);
        printf("Parallel (%d threads): %.2f ms\n", thread_counts[i], timer_elapsed_ms(&timer));
    }

    free(original);
    free(arr);
    free(temp);
    return 0;
}
