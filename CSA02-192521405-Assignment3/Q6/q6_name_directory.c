#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 30

long long cmp_calls = 0;
long long char_copies = 0;

int my_strcmp(const char *s1, const char *s2)
{
    cmp_calls++;
    return strcmp(s1, s2);
}

int my_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++; s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

void my_strcpy(char *dst, const char *src)
{
    char_copies += (long long)(strlen(src) + 1);
    strcpy(dst, src);
}

/* Direct string swap sort (Selection Sort for exact swap/copy counting) */
void sort_direct(char names[][MAX_LEN], int n)
{
    cmp_calls = 0;
    char_copies = 0;
    char tmp[MAX_LEN];
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (my_strcmp(names[j], names[min_idx]) < 0) min_idx = j;
        }
        if (min_idx != i) {
            my_strcpy(tmp, names[i]);
            my_strcpy(names[i], names[min_idx]);
            my_strcpy(names[min_idx], tmp);
        }
    }
}

/* Indirect index sort: 0 character copies */
void sort_indices(char names[][MAX_LEN], int idx[], int n)
{
    cmp_calls = 0;
    char_copies = 0;
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (my_strcmp(names[idx[j]], names[idx[min_idx]]) < 0) min_idx = j;
        }
        if (min_idx != i) {
            int t = idx[i]; idx[i] = idx[min_idx]; idx[min_idx] = t;
        }
    }
}

int binary_search_names(char names[][MAX_LEN], const int idx[], int n, const char *target, int *search_cmps)
{
    *search_cmps = 0;
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        (*search_cmps)++;
        int c = strcmp(names[idx[mid]], target);
        if (c == 0) return mid;
        else if (c < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int main(void)
{
    /* 1. Demonstrate 500-resident directory, case sensitivity, duplicates, and binary search */
    char sample[7][MAX_LEN] = {
        "Padma Dharshini", "Ananya Rao", "apple", "Apple", "Vikram Nair", "Ananya Rao", "Karthik S"
    };
    int s_idx[7];
    sort_indices(sample, s_idx, 7);
    printf("=== Q6: SORTED SAMPLE DIRECTORY & DUPLICATES ===\n");
    for (int i = 0; i < 7; i++) {
        printf("  [%d] %s", i, sample[s_idx[i]]);
        if (i > 0 && strcmp(sample[s_idx[i]], sample[s_idx[i - 1]]) == 0) {
            printf("  <-- DUPLICATE");
        }
        printf("\n");
    }

    int sc = 0;
    int p1 = binary_search_names(sample, s_idx, 7, "Vikram Nair", &sc);
    printf("Search \"Vikram Nair\" -> sorted pos %d, strcmp calls = %d\n", p1, sc);
    int p2 = binary_search_names(sample, s_idx, 7, "Zoya Khan", &sc);
    printf("Search \"Zoya Khan\"   -> sorted pos %d, strcmp calls = %d\n", p2, sc);

    /* 2. 29-char vs 30-char name boundary test */
    char name29[MAX_LEN] = "Abcdefghijklmnopqrstuvwxy1234"; /* 29 chars + '\0' = 30 bytes */
    char name30[MAX_LEN];
    strncpy(name30, "Abcdefghijklmnopqrstuvwxy12345", MAX_LEN); /* 30 chars, NO '\0' */
    printf("\nBoundary Test: strlen(name29) = %zu (safe); name30 lacks null terminator in char[30]!\n",
           strlen(name29));

    /* 3. Benchmark for N = 100, 500, 5000 */
    int sizes[] = {100, 500, 5000};
    printf("\n=== BENCHMARK: DIRECT CHAR COPYING VS INDEX SORTING ===\n");
    for (int k = 0; k < 3; k++) {
        int n = sizes[k];
        char (*arr1)[MAX_LEN] = malloc((size_t)n * MAX_LEN);
        char (*arr2)[MAX_LEN] = malloc((size_t)n * MAX_LEN);
        int *idx = malloc((size_t)n * sizeof(int));

        srand(99);
        for (int i = 0; i < n; i++) {
            snprintf(arr1[i], MAX_LEN, "Resident_%05d_HostelBlockA", rand() % (n * 10));
            strcpy(arr2[i], arr1[i]);
        }

        sort_direct(arr1, n);
        long long d_cmp = cmp_calls, d_cpy = char_copies;

        sort_indices(arr2, idx, n);
        long long i_cmp = cmp_calls, i_cpy = char_copies;

        printf("N = %4d | Direct: strcmp=%8lld, char_copies=%9lld | Index: strcmp=%8lld, char_copies=%lld\n",
               n, d_cmp, d_cpy, i_cmp, i_cpy);

        free(arr1); free(arr2); free(idx);
    }
    return 0;
}
