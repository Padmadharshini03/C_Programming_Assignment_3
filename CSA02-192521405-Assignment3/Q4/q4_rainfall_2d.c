#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISTRICTS 38
#define MONTHS    12

void compute_district_totals(int rows, int cols, double rain[rows][cols], double totals[])
{
    for (int i = 0; i < rows; i++) {
        double sum = 0.0;
        for (int j = 0; j < cols; j++) {
            sum += rain[i][j];
        }
        totals[i] = sum;
    }
}

void compute_monthly_averages(int rows, int cols, double rain[rows][cols], double month_avg[])
{
    for (int j = 0; j < cols; j++) {
        double sum = 0.0;
        for (int i = 0; i < rows; i++) {
            sum += rain[i][j];
        }
        month_avg[j] = sum / rows;
    }
}

void find_wettest(int rows, int cols, double rain[rows][cols], int *best_d, int *best_m, double *max_val)
{
    *best_d = 0; *best_m = 0; *max_val = rain[0][0];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (rain[i][j] > *max_val) {
                *max_val = rain[i][j];
                *best_d = i;
                *best_m = j;
            }
        }
    }
}

void report_deficient(int rows, const double totals[])
{
    double state_sum = 0.0;
    for (int i = 0; i < rows; i++) state_sum += totals[i];
    double state_avg = state_sum / rows;
    double threshold = 0.60 * state_avg;
    printf("State Annual Average = %.2f mm | 60%% Threshold = %.2f mm\n", state_avg, threshold);
    printf("Deficient Districts (< 60%% of state average): ");
    int found = 0;
    for (int i = 0; i < rows; i++) {
        if (totals[i] < threshold) {
            printf("D%d (%.1f mm) ", i + 1, totals[i]);
            found++;
        }
    }
    if (!found) printf("None");
    printf("\n");
}

int main(void)
{
    /* 1. Hand-verified 3 x 4 sample */
    double sample[3][4] = {
        {120.0, 150.0, 180.0, 150.0}, /* Total = 600.0 */
        {200.0, 250.0, 300.0, 250.0}, /* Total = 1000.0 */
        { 40.0,  50.0,  60.0,  50.0}  /* Total = 200.0 (Deficient: < 0.6 * 600 = 360) */
    };
    double tot[3], mavg[4], max_v;
    int bd, bm;
    printf("=== Q4: HAND-VERIFIED 3x4 SAMPLE ===\n");
    compute_district_totals(3, 4, sample, tot);
    compute_monthly_averages(3, 4, sample, mavg);
    find_wettest(3, 4, sample, &bd, &bm, &max_v);
    for (int i = 0; i < 3; i++) printf("District %d Annual Total: %.2f mm\n", i + 1, tot[i]);
    printf("Monthly Averages: ");
    for (int j = 0; j < 4; j++) printf("M%d=%.2f ", j + 1, mavg[j]);
    printf("\nWettest District-Month: D%d, M%d (%.2f mm)\n", bd + 1, bm + 1, max_v);
    report_deficient(3, tot);

    /* 2. Full 38 x 12 Tamil Nadu districts simulation */
    double rain[DISTRICTS][MONTHS];
    srand(108);
    for (int i = 0; i < DISTRICTS; i++) {
        double scale = (i == 5 || i == 22) ? 0.35 : 1.0; /* Make 2 districts deficient */
        for (int j = 0; j < MONTHS; j++) {
            rain[i][j] = scale * (40.0 + (rand() % 160));
        }
    }
    double tot38[DISTRICTS], mavg12[MONTHS];
    printf("\n=== FULL 38x12 STATE DISASTER MANAGEMENT REPORT ===\n");
    compute_district_totals(DISTRICTS, MONTHS, rain, tot38);
    compute_monthly_averages(DISTRICTS, MONTHS, rain, mavg12);
    find_wettest(DISTRICTS, MONTHS, rain, &bd, &bm, &max_v);
    printf("Wettest District-Month: District %d, Month %d (%.2f mm)\n", bd + 1, bm + 1, max_v);
    report_deficient(DISTRICTS, tot38);

    /* 3. Cache Locality Benchmark: 2000 x 2000 Row-wise vs Column-wise */
    const int DIM = 2000;
    double (*big)[2000] = malloc(sizeof(double[2000][2000]));
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            big[i][j] = 1.0;

    volatile double sink = 0.0;
    clock_t t0 = clock();
    for (int rep = 0; rep < 25; rep++) {
        double s = 0.0;
        for (int i = 0; i < DIM; i++)
            for (int j = 0; j < DIM; j++)
                s += big[i][j];
        sink += s;
    }
    clock_t t1 = clock();

    for (int rep = 0; rep < 25; rep++) {
        double s = 0.0;
        for (int j = 0; j < DIM; j++)
            for (int i = 0; i < DIM; i++)
                s += big[i][j];
        sink += s;
    }
    clock_t t2 = clock();

    double row_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;
    double col_ms = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("\n=== CACHE LOCALITY BENCHMARK (2000x2000 x 25 passes) ===\n");
    printf("Row-wise traversal time    : %.2f ms\n", row_ms);
    printf("Column-wise traversal time : %.2f ms (%.2fx slower)\n", col_ms, col_ms / (row_ms > 0 ? row_ms : 1.0));
    free(big);
    return 0;
}
