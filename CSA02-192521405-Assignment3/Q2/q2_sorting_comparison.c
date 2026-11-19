#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long long comps;
    long long moves; /* swaps or shifts */
} Metrics;

Metrics bubble_sort(int a[], int n)
{
    Metrics m = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            m.comps++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                m.moves++;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    return m;
}

Metrics selection_sort(int a[], int n)
{
    Metrics m = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            m.comps++;
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = a[i];
            a[i] = a[min_idx];
            a[min_idx] = tmp;
            m.moves++;
        }
    }
    return m;
}

Metrics insertion_sort(int a[], int n)
{
    Metrics m = {0, 0};
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            m.comps++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                m.moves++;
                j--;
            } else {
                break;
            }
        }
        a[j + 1] = key;
    }
    return m;
}

void generate_data(int base[], int n, int mode)
{
    /* mode 0: Sorted, 1: Nearly Sorted (20 late registrations), 2: Reverse, 3: Random */
    for (int i = 0; i < n; i++) base[i] = 192521000 + i;
    if (mode == 1) {
        int k = (n < 20) ? (n / 3) : 20;
        for (int t = 0; t < k; t++) {
            int idx = (t * (n / k)) % n;
            base[idx] += 500;
        }
    } else if (mode == 2) {
        for (int i = 0; i < n; i++) base[i] = 192521000 + (n - i);
    } else if (mode == 3) {
        srand(42);
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = base[i]; base[i] = base[j]; base[j] = tmp;
        }
    }
}

int main(void)
{
    int sizes[] = {100, 1000, 10000};
    const char *modes[] = {"Sorted", "Nearly Sorted", "Reverse", "Random"};

    printf("=== Q2: EXAMINATION SEATING REGISTER SORTING BENCHMARK ===\n");
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        int *base = malloc((size_t)n * sizeof(int));
        int *work = malloc((size_t)n * sizeof(int));
        printf("\n--- Input Size N = %d ---\n", n);
        printf("%-14s | %-22s | %-22s | %-22s\n",
               "Order", "Bubble (Comp / Swap)", "Selection (Comp / Swap)", "Insertion (Comp / Shift)");
        printf("---------------+------------------------+-------------------------+-------------------------\n");
        for (int m = 0; m < 4; m++) {
            generate_data(base, n, m);

            memcpy(work, base, (size_t)n * sizeof(int));
            Metrics mb = bubble_sort(work, n);

            memcpy(work, base, (size_t)n * sizeof(int));
            Metrics ms = selection_sort(work, n);

            memcpy(work, base, (size_t)n * sizeof(int));
            Metrics mi = insertion_sort(work, n);

            printf("%-14s | %10lld / %-9lld | %11lld / %-9lld | %11lld / %-9lld\n",
                   modes[m], mb.comps, mb.moves, ms.comps, ms.moves, mi.comps, mi.moves);
        }
        free(base);
        free(work);
    }
    return 0;
}
