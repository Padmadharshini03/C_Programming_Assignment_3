#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long cmp_count = 0;

int merge_desc(const int a[], int na, const int b[], int nb, int out[])
{
    cmp_count = 0;
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        cmp_count++;
        if (a[i] >= b[j]) out[k++] = a[i++];
        else out[k++] = b[j++];
    }
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
    return k;
}

int desc_cmp(const void *p1, const void *p2)
{
    cmp_count++;
    int x = *(const int *)p1;
    int y = *(const int *)p2;
    return (y > x) - (y < x);
}

int main(void)
{
    int sizes[] = {1000, 10000};
    printf("=== Q3: MERGE VS CONCATENATE + QSORT COMPARISON ===\n");
    for (int s = 0; s < 2; s++) {
        int n = sizes[s];
        int *a = malloc((size_t)n * sizeof(int));
        int *b = malloc((size_t)n * sizeof(int));
        int *out = malloc((size_t)(2 * n) * sizeof(int));

        /* Interleaved descending scores */
        for (int i = 0; i < n; i++) {
            a[i] = 2 * n - 2 * i;
            b[i] = 2 * n - 2 * i - 1;
        }
        merge_desc(a, n, b, n, out);
        long long merge_cmps = cmp_count;

        /* Join + qsort */
        memcpy(out, a, (size_t)n * sizeof(int));
        memcpy(out + n, b, (size_t)n * sizeof(int));
        cmp_count = 0;
        qsort(out, (size_t)(2 * n), sizeof(int), desc_cmp);
        long long qsort_cmps = cmp_count;

        printf("m = n = %5d -> Merge Comparisons = %7lld | Join + qsort() Comparisons = %8lld\n",
               n, merge_cmps, qsort_cmps);

        free(a); free(b); free(out);
    }

    /* All equal scores edge case */
    int eqA[4] = {90, 90, 90, 90}, eqB[3] = {90, 90, 90}, eqOut[7];
    merge_desc(eqA, 4, eqB, 3, eqOut);
    printf("All equal (m=4, n=3) -> Comparisons = %lld (minimum possible = min(m,n) = 3)\n", cmp_count);
    return 0;
}
