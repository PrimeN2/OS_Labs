#include "parallel_merge_sort.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *arr;
    int *temp;
    int left;
    int right;
    int num_threads;
} SortArgs;

void merge(int arr[], int temp[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; i++) arr[i] = temp[i];
}

void merge_sort_sequential(int arr[], int temp[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_sequential(arr, temp, left, mid);
    merge_sort_sequential(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}

void* thread_merge_sort(void* arg) {
    SortArgs* args = (SortArgs*)arg;
    if (args->num_threads <= 1) {
        merge_sort_sequential(args->arr, args->temp, args->left, args->right);
        return NULL;
    }

    int mid = args->left + (args->right - args->left) / 2;
    pthread_t thread;

    SortArgs left_args = {args->arr, args->temp, args->left, mid, args->num_threads / 2};
    SortArgs right_args = {args->arr, args->temp, mid + 1, args->right, args->num_threads - args->num_threads / 2};

    pthread_create(&thread, NULL, thread_merge_sort, &right_args);
    thread_merge_sort(&left_args);
    pthread_join(thread, NULL);

    merge(args->arr, args->temp, args->left, mid, args->right);
    return NULL;
}

void merge_sort_parallel(int arr[], int temp[], int left, int right, int num_threads) {
    SortArgs args = {arr, temp, left, right, num_threads};
    thread_merge_sort(&args);
}
