#include "contracts.h"
#include <stdlib.h>

static void quicksort(int *arr, size_t left, size_t right) {
    if (left >= right) return;
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    int temp;

    while (i <= j) {
        while (arr[i] < pivot) ++i;
        while (arr[j] > pivot) --j;
        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            ++i;
            --j;
        }
    }

    if (left < j) quicksort(arr, left, j);
    if (i < right) quicksort(arr, i, right);
}

int *sort(int *array, size_t n) {
    int *arr = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; ++i) {
        arr[i] = array[i];
    }
    if (n > 1) {
        quicksort(arr, 0, n - 1);
    }
    return arr;
}
