#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---- header / project constants ---- */
#define STUDENT_ID_HEADER "00001"            /* <-- put your own student ID here */
#define PROJECT_TITLE     "Dynamic Text Mining and Linguistic Pattern Analyzer"
#define CUSTOM_ANALYSIS   "Lexical Diversity Score"
#define PARAGRAPH_FILE    "paragraph.txt"
#define DEFAULT_SOURCE    "Alice's Adventures in Wonderland, Chapter I - Lewis Carroll (1865, public domain)"

#define WORD_MAX          40    /* 39 characters + '\0'                         */
#define INITIAL_TEXT_CAP  256   /* bytes; the text buffer doubles when full     */
#define INITIAL_STATS_CAP 16    /* entries; the stats array doubles when full   */
#define SOURCE_MAX        200

/* One distinct word and its frequency. */
struct wordStat {
    char name[WORD_MAX];
    int  count;
};
typedef struct wordStat WordStat;

/* Everything the analyzer owns. All heap memory hangs off this struct. */
typedef struct Analyzer {
    char     *text;            /* dynamically allocated text buffer          */
    size_t    text_len;        /* bytes used (without the '\0')              */
    size_t    text_cap;        /* bytes allocated                            */
    WordStat *stats;           /* dynamically allocated word-statistics array*/
    int       distinct;        /* number of distinct words stored            */
    int       capacity;        /* entries allocated                          */
    long      total_words;     /* total words found in the text              */
    int       truncated_words; /* words cut to 39 characters                 */
    int       selected;        /* index of the "selected" entry, -1 = none   */
    int       running;         /* 0 once the user chooses Exit               */
    char      source[SOURCE_MAX];
} Analyzer;

/* Every menu operation has this signature so it fits in one array. */
typedef void (*Operation)(Analyzer *);

/* text_input.c */
void read_line(const char *prompt, char *buf, size_t size);
int  read_int(const char *prompt, int *out);
void text_reset(Analyzer *a);
int  text_append(Analyzer *a, const char *chunk, size_t n);
void load_text_from_file(Analyzer *a);
void read_pasted_text(Analyzer *a);

/* word_extract.c */
const char *scan_word(const char *p, char *word, int *truncated);
int  analyze_text(Analyzer *a);

/* operations.c  (required operations) */
void count_words(Analyzer *a);
void longest_word(Analyzer *a);
void most_frequent(Analyzer *a);
void display_word_statistics(Analyzer *a);
void sort_word_statistics(Analyzer *a);
void search_word(Analyzer *a);
void generate_text_summary(Analyzer *a);
void terminate_program(Analyzer *a);

/* custom_analysis.c */
double average_word_length(const Analyzer *a);
double lexical_diversity(const Analyzer *a);
const char *diversity_label(double score);
void custom_analysis(Analyzer *a);
void memory_inspection(Analyzer *a);

#endif /* MAIN_H */