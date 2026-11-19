# CSA02 – C Programming: Assignment 3 (CO3)

- **Student Name:** Padma Dharshini S D
- **Register Number:** 192521405
- **Programme:** B.E / B.Tech
- **Course Code & Name:** CSA02 – C Programming
- **Course Outcome:** CO3 (Searching, Sorting, Merging, 1D/2D/Multi-D Arrays, Character Arrays & String Handling)
- **Compiler & Version:** GCC 13.x (`gcc -Wall -Wextra -std=c11`)

## Folder Structure & Compilation Commands

| Folder | Source Files | Compile & Run Command | Summary of Result |
| :--- | :--- | :--- | :--- |
| `Q1/` | `q1_complete_code.c`, `q1_search_analysis.c` | `gcc -Wall -Wextra q1_complete_code.c -o q1 && ./q1` | Binary search finds keys in `ceil(log2(N+1))` comparisons (17 max for 100,000 books) vs. 100,000 worst-case for linear search. |
| `Q2/` | `q2_sorting_comparison.c` | `gcc -Wall -Wextra q2_sorting_comparison.c -o q2 && ./q2` | Insertion sort outperforms Bubble and Selection sort on sorted and nearly sorted seating registers (`O(n + d)`). |
| `Q3/` | `q3_complete_code.c`, `q3_merge_analysis.c` | `gcc -Wall -Wextra q3_complete_code.c -o q3 && ./q3` | Two-pointer merge combines sorted merit lists in `O(m + n)` time (at most `m + n - 1` comparisons), preserving campus order on ties. |
| `Q4/` | `q4_rainfall_2d.c` | `gcc -Wall -Wextra -O2 q4_rainfall_2d.c -o q4 && ./q4` | Computes district totals, state monthly averages, wettest cell, and `<60%` deficient districts; row-wise traversal beats column-wise due to spatial cache locality. |
| `Q5/` | `q5_complete_code.c`, `q5_string_comparison.c` | `gcc -Wall -Wextra q5_complete_code.c -o q5 && ./q5` | Manual null-terminated loops match `<string.h>` behavior; bounded `snprintf`/`fgets` prevent buffer overflows. |
| `Q6/` | `q6_name_directory.c` | `gcc -Wall -Wextra q6_name_directory.c -o q6 && ./q6` | Sorting an integer index array eliminates all `strcpy` character movement (`0` byte copies vs. millions of character shifts). |
| `Q7/` | `q7_election_counter.c` | `gcc -Wall -Wextra -O2 q7_election_counter.c -o q7 && ./q7` | Single-pass frequency array tallies 500,000 ballots in `< 2 ms` (`O(v + c log c)`), filters invalid votes, and detects ties cleanly. |
