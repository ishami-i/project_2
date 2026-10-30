# Project 2: Dynamic Memory, Pointers and Function Pointers in C

Three C programs that use structures, dynamic memory allocation, pointer traversal and function pointers. Each one is solved in its own directory with its own README.

## Repository structure

```
.
├── README.md
├── .gitignore
├── academic_performance/
│   ├── README.md
│   ├── main.h
│   ├── main.c
│   ├── input.c
│   ├── add_student.c
│   ├── display.c
│   ├── sorting.c
│   ├── averaging.c
│   ├── top_student.c
│   ├── search.c
│   ├── remove.c
│   ├── custom_analysis.c
│   ├── summary.c
│   ├── students.txt
│   ├── compiler.sh
│   └── sample_output.txt
├── text_mining/
│   ├── README.md
│   ├── main.h
│   ├── main.c
│   ├── text_input.c
│   ├── word_extract.c
│   ├── operations.c
│   ├── analysis.c
│   ├── paragraph.txt
│   ├── compiler.sh
│   └── sample_output.txt
└── IoT_event_monitoring/
    ├── README.md
    ├── main.h
    ├── main.c
    ├── input.c
    ├── devices.c
    ├── callbacks.c
    ├── simulation.c
    ├── report.c
    ├── compiler.sh
    └── sample_output.txt
```

## The three problems

| # | Directory | Topic | Custom feature |
|---|-----------|-------|----------------|
| 1 | [`academic_performance`](academic_performance/) | Student records with growing and shrinking storage | Academic Risk Index |
| 2 | [`text_mining`](text_mining/) | Word statistics of a paragraph | Lexical Diversity Score |
| 3 | [`IoT_event_monitoring`](IoT_event_monitoring/) | IoT devices processed by callbacks | Sensor Drift Detector |

### 1. Dynamic academic performance and risk analysis
A menu-driven record system that adds, removes, sorts, searches and summarises student records stored in a dynamically resized array. The custom analysis calculates a risk score from each student's GPA.

### 2. Dynamic text mining and linguistic pattern analyzer
Reads a paragraph (from a file or pasted), extracts words with pointer traversal, stores each distinct word and its frequency in a growing array and reports statistics such as the longest word, the most frequent word and the lexical diversity. It also prints the memory addresses of its main data structures.

### 3. Dynamic IoT event monitoring and callback engine
Registers devices dynamically, simulates temperature, pressure and battery readings, and processes each reading through a callback selected with a function pointer. A custom callback detects sudden sensor drift.

## Concepts demonstrated

| Concept | Where |
|---------|-------|
| `malloc` / `calloc` / `realloc` / `free` | All three: growing and shrinking arrays, text buffer, alert log |
| Pointer traversal | Display, averaging and top student (1); word extraction (2); device loop (3) |
| Function pointers | Menu dispatch tables in all three; callback table in (3); `qsort` comparators in (1) and (2) |
| `union` | Device readings in (3) |
| String functions | `strcmp`, `strlen`, `strcpy` in (2); `strcmp`, `strcasecmp` in (1) |

## Requirements

A C compiler such as `gcc` or `clang` (C11).

## Building and running

Each directory is built on its own with the `compiler.sh` script it contains. From the repository root:

```bash
# 1. Academic performance
cd academic_performance && bash compiler.sh && ./academic && cd ..

# 2. Text mining
cd text_mining && bash compiler.sh && ./text_mining_app && cd ..

# 3. IoT event monitoring
cd IoT_event_monitoring && bash compiler.sh && ./iot_monitor && cd ..
```

Run the programs from inside their own directory, because `students.txt` and `paragraph.txt` are read from the current directory.

## Notes

- Set `STUDENT_ID_HEADER` in each `main.h` to your own student ID; it is `00001` as a placeholder.
- Compiled binaries (`academic`, `text_mining_app`, `iot_monitor`) and `.dSYM` folders are build output and are not committed.
- Each `sample_output.txt` is a real run with the typed input not echoed.
- Open each directory's own README for design notes, formulas and sample runs.