#include <stdio.h>
#include <stdlib.h>

long steps;

int linear_search(const int a[], int n, int key)
{
    steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;
        if (a[i] == key) return i;
    }
    return -1;
}

int binary_search(const int a[], int n, int key)
{
    steps = 0;
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        steps++;
        if (a[mid] == key) return mid;
        else if (a[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int interpolation_search(const int a[], int n, int key)
{
    steps = 0;
    int low = 0, high = n - 1;
    while (low <= high && key >= a[low] && key <= a[high]) {
        if (low == high) {
            steps++;
            return (a[low] == key) ? low : -1;
        }
        int pos = low + (int)(((double)(high - low) / (a[high] - a[low])) * (key - a[low]));
        steps++;
        if (a[pos] == key) return pos;
        if (a[pos] < key) low = pos + 1;
        else high = pos - 1;
    }
    return -1;
}

int main(void)
{
    int sizes[] = {10, 1000, 10000, 100000};
    printf("=== Q1: LINEAR VS BINARY VS INTERPOLATION SEARCH ===\n");
    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        int *arr = malloc((size_t)n * sizeof(int));
        for (int i = 0; i < n; i++) arr[i] = 1000 + i * 3; /* Uniformly spaced sorted accession numbers */

        int k_begin = arr[0];
        int k_mid   = arr[n / 2];
        int k_end   = arr[n - 1];
        int k_miss  = arr[n / 2] + 1; /* Guaranteed missing */

        printf("\n--- Catalogue Size N = %d ---\n", n);
        linear_search(arr, n, k_begin); long l_beg = steps;
        linear_search(arr, n, k_mid);   long l_mid = steps;
        linear_search(arr, n, k_end);   long l_end = steps;
        linear_search(arr, n, k_miss);  long l_mis = steps;
        printf("Linear Search        : Begin=%6ld | Mid=%6ld | End=%6ld | Missing=%6ld\n",
               l_beg, l_mid, l_end, l_mis);

        binary_search(arr, n, k_begin); long b_beg = steps;
        binary_search(arr, n, k_mid);   long b_mid = steps;
        binary_search(arr, n, k_end);   long b_end = steps;
        binary_search(arr, n, k_miss);  long b_mis = steps;
        printf("Binary Search        : Begin=%6ld | Mid=%6ld | End=%6ld | Missing=%6ld\n",
               b_beg, b_mid, b_end, b_mis);

        interpolation_search(arr, n, k_begin); long i_beg = steps;
        interpolation_search(arr, n, k_mid);   long i_mid = steps;
        interpolation_search(arr, n, k_end);   long i_end = steps;
        interpolation_search(arr, n, k_miss);  long i_mis = steps;
        printf("Interpolation Search : Begin=%6ld | Mid=%6ld | End=%6ld | Missing=%6ld\n",
               i_beg, i_mid, i_end, i_mis);

        free(arr);
    }
    return 0;
}
