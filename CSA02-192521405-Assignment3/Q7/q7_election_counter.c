#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CANDIDATES 12

typedef struct {
    int id;
    long votes;
} CandidateResult;

void tally_and_report(int num_candidates, const int votes[], long num_votes, int verbose)
{
    long count[MAX_CANDIDATES + 1] = {0}; /* 1-based indexing: 1..num_candidates */
    long valid = 0, invalid = 0;

    /* Single pass O(v) counting */
    for (long i = 0; i < num_votes; i++) {
        int v = votes[i];
        if (v >= 1 && v <= num_candidates) {
            count[v]++;
            valid++;
        } else {
            invalid++;
        }
    }

    CandidateResult res[MAX_CANDIDATES];
    for (int c = 0; c < num_candidates; c++) {
        res[c].id = c + 1;
        res[c].votes = count[c + 1];
    }

    /* Descending insertion sort on c <= 12 candidates */
    for (int i = 1; i < num_candidates; i++) {
        CandidateResult key = res[i];
        int j = i - 1;
        while (j >= 0 && res[j].votes < key.votes) {
            res[j + 1] = res[j];
            j--;
        }
        res[j + 1] = key;
    }

    if (verbose) {
        printf("Total Ballots: %ld | Valid: %ld | Invalid: %ld\n", num_votes, valid, invalid);
        for (int c = 0; c < num_candidates; c++) {
            printf("  Rank %2d: Candidate #%d -> %ld votes\n", c + 1, res[c].id, res[c].votes);
        }
        if (valid == 0) {
            printf("Result: No valid votes cast.\n\n");
        } else if (num_candidates > 1 && res[0].votes == res[1].votes) {
            printf("Result: TIE for 1st place with %ld votes among Candidates:", res[0].votes);
            for (int c = 0; c < num_candidates && res[c].votes == res[0].votes; c++) {
                printf(" #%d", res[c].id);
            }
            printf("\n\n");
        } else {
            printf("Result: WINNER is Candidate #%d with %ld votes!\n\n", res[0].id, res[0].votes);
        }
    }
}

int main(void)
{
    /* 1. Hand-verified small vote set with invalid votes and a tie */
    int small_votes[] = {1, 2, 1, 0, -3, 2, 5, 3, 1, 2}; /* 4 candidates: C1=3, C2=3, C3=1, C4=0, Invalid=3 */
    printf("=== Q7: HAND-VERIFIED SMALL ELECTION SET (4 Candidates, 10 Ballots) ===\n");
    tally_and_report(4, small_votes, 10, 1);

    /* 2. Timing benchmark for 5,000 and 500,000 votes across 12 candidates */
    long test_sizes[] = {5000, 500000};
    printf("=== SCALABILITY BENCHMARK (12 Candidates) ===\n");
    for (int t = 0; t < 2; t++) {
        long n = test_sizes[t];
        int *ballots = malloc((size_t)n * sizeof(int));
        srand(77);
        for (long i = 0; i < n; i++) {
            ballots[i] = (rand() % 14); /* Generates 0..13 so 0 and 13 are invalid */
        }
        clock_t start = clock();
        tally_and_report(12, ballots, n, 1);
        clock_t end = clock();
        double ms = 1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
        printf("Execution Time for %ld votes = %.3f ms\n\n", n, ms);
        free(ballots);
    }
    return 0;
}
