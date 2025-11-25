#include "contracts.h"
#include <stdlib.h>

int *sort(int *array, size_t n) {
    int *arr = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; ++i) {
        arr[i] = array[i];
    }
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}
