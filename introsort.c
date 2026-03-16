#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define INSERTION_SORT_THRESHOLD 16

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static size_t floor_log2_size_t(size_t n) {
    size_t log = 0;
    while (n > 1) {
        n >>= 1;
        ++log;
    }
    return log;
}

static size_t median_of_three(int *arr, size_t a, size_t b, size_t c) {
    if (arr[a] > arr[b]) swap_int(&arr[a], &arr[b]);
    if (arr[a] > arr[c]) swap_int(&arr[a], &arr[c]);
    if (arr[b] > arr[c]) swap_int(&arr[b], &arr[c]);
    return b;
}

static void insertion_sort_range(int *arr, size_t low, size_t high) {
    for (size_t i = low + 1; i <= high; ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > low && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

static void heapify(int *arr, size_t base, size_t heap_size, size_t root) {
    while (1) {
        size_t largest = root;
        size_t left = 2 * root + 1;
        size_t right = 2 * root + 2;

        if (left < heap_size && arr[base + left] > arr[base + largest]) {
            largest = left;
        }
        if (right < heap_size && arr[base + right] > arr[base + largest]) {
            largest = right;
        }

        if (largest == root) {
            break;
        }

        swap_int(&arr[base + root], &arr[base + largest]);
        root = largest;
    }
}

static void heapsort_range(int *arr, size_t low, size_t high) {
    size_t n = high - low + 1;

    for (size_t i = n / 2; i > 0; --i) {
        heapify(arr, low, n, i - 1);
    }

    for (size_t i = n; i > 1; --i) {
        swap_int(&arr[low], &arr[low + i - 1]);
        heapify(arr, low, i - 1, 0);
    }
}

static size_t partition(int *arr, size_t low, size_t high) {
    size_t mid = low + (high - low) / 2;
    size_t pivot_idx = median_of_three(arr, low, mid, high);
    int pivot = arr[pivot_idx];
    swap_int(&arr[pivot_idx], &arr[high]);

    size_t store = low;
    for (size_t i = low; i < high; ++i) {
        if (arr[i] < pivot) {
            swap_int(&arr[i], &arr[store]);
            ++store;
        }
    }
    swap_int(&arr[store], &arr[high]);
    return store;
}

static void introsort_util(int *arr, size_t low, size_t high, size_t depth_limit) {
    while (high > low) {
        size_t size = high - low + 1;

        if (size <= INSERTION_SORT_THRESHOLD) {
            insertion_sort_range(arr, low, high);
            return;
        }

        if (depth_limit == 0) {
            heapsort_range(arr, low, high);
            return;
        }

        --depth_limit;
        size_t pivot = partition(arr, low, high);

        if (pivot - low < high - pivot) {
            if (pivot > low) {
                introsort_util(arr, low, pivot - 1, depth_limit);
            }
            low = pivot + 1;
        } else {
            if (pivot < high) {
                introsort_util(arr, pivot + 1, high, depth_limit);
            }
            if (pivot == 0) return;
            high = pivot - 1;
        }
    }
}

void introsort(int *arr, size_t n) {
    if (arr == NULL || n < 2) {
        return;
    }

    size_t depth_limit = 2 * floor_log2_size_t(n);
    introsort_util(arr, 0, n - 1, depth_limit);
}

static int is_sorted(const int *arr, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int data[] = {42, 5, 1, 99, -3, 0, 88, 7, 7, 19, 15, 2, 63, 4, 3};
    size_t n = sizeof(data) / sizeof(data[0]);

    introsort(data, n);

    printf("Sorted result: ");
    for (size_t i = 0; i < n; ++i) {
        printf("%d%s", data[i], (i + 1 == n) ? "\n" : " ");
    }

    if (!is_sorted(data, n)) {
        fprintf(stderr, "Sort verification failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
