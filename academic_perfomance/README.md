# Dynamic Academic Performance and Risk Analysis Engine

A menu-driven student record system written in C. Records live in a dynamically allocated array that grows when students are added and shrinks when they are removed. The program evaluates performance and flags students who may be at academic risk.

## Files

| File | Purpose |
|------|---------|
| `main.h` | Shared header: constants, `Student` and `Database` structures, function prototypes |
| `main.c` | Header output, menu and the function-pointer dispatch table |
| `input.c` | Safe input helpers (`read_line`, `read_int`, `read_float`) and loading of `students.txt` |
| `add_student.c` | `add_student`: validation, duplicate-ID check, array growth |
| `display.c` | `display_students`: formatted table (pointer traversal) |
| `sorting.c` | `sort_students`: by grade, name or ID using `qsort` and `strcmp` |
| `averaging.c` | `calculate_average` and the per-course averages |
| `top_student.c` | `find_top_student` (handles ties) |
| `search.c` | `search_student` by ID or name (case-insensitive) |
| `remove.c` | `remove_student`: removal and array shrinking |
| `custom_analysis.c` | Academic Risk Index |
| `summary.c` | `generate_summary_report` |
| `students.txt` | Initial dataset of 15 students |
| `sample_output.txt` | A complete sample run |

## Features

- Initial dataset of 15 students loaded into memory allocated with `calloc`.
- Capacity doubles with `realloc` when full and halves when fewer than half the slots are used.
- Rejects duplicate IDs, non-numeric input, IDs that are not positive and grades outside 0 to 100 (including `nan`).
- All allocation results are checked; all memory is freed before exit.
- Displaying, averaging and finding the top student traverse the records with pointers.
- Header shows the student ID, project title and custom analysis.

## Menu

```
 1. Add a new student record
 2. Display all student records
 3. Sort students (grade / name / ID)
 4. Calculate average grade
 5. Identify top-performing student
 6. Search for a student (ID / name)
 7. Remove a student record
 8. Run custom academic analysis (Risk Index)
 9. Display summary report
10. Exit
```

## Build and run

```bash
bash compiler.sh
./academic
```

or directly:

```bash
gcc -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE -g main.c input.c add_student.c display.c sorting.c averaging.c top_student.c search.c remove.c custom_analysis.c summary.c -o academic
./academic
```

## Custom analysis: Academic Risk Index

**Purpose:** give each student a score from 0 to 100 showing how far their GPA is below the safe level.

```
GPA  = grade * 5 / 100
risk = 0                         if GPA >= 3.5
risk = (3.5 - GPA) / 3.5 * 100   if GPA <  3.5
```

| Level | Condition |
|-------|-----------|
| SAFE | GPA 3.5 or higher |
| CLOSE TO RISK | GPA 3.0 to below 3.5 |
| AT RISK | GPA below 3.0 (grade below 60) |

**Interpretation:** 0 means safe; the closer to 100, the more urgent. Contact AT RISK students first and keep an eye on CLOSE TO RISK students. The pass threshold for the summary report is grade 60 (GPA 3.0).

## Sample session

```
Student ID: 00001
Project: Dynamic Academic Performance and Risk Analysis Engine
Custom Analysis: Academic Risk Index
Loaded 15 students from students.txt.

Enter your choice: 1
Enter student ID (positive number): 16
...
[memory] capacity grew from 15 to 30 records

Enter your choice: 1
Enter student ID (positive number): 16
Error: a student with ID 16 already exists.

Enter your choice: 7
Enter the ID of the student to remove: 16
Student with ID 16 removed successfully. Total records: 15
...
[memory] capacity shrank from 30 to 15 records

Enter your choice: 9
========== SUMMARY REPORT ==========
Total student records : 16
Average grade         : 82.12
Highest grade         : 95.00 (Sarah White)
Lowest grade          : 55.00 (Test Student)
Pass threshold        : 60
Students passing      : 15
Students below        : 1
```

The full run, including the rejected duplicate ID, the invalid grade and the removals, is in `sample_output.txt`.

## Design notes

- `Database` bundles the array, its count and capacity, so every operation receives one pointer and cannot lose track of the size.
- Every menu operation has the signature `void (*)(Database *)`, so one array of function pointers dispatches `actions[choice - 1](&db)`.
- Input is read with `fgets` and converted with `strtol` / `strtof`, so over-long input cannot overflow a buffer.
