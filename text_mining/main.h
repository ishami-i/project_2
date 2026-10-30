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

// One distinct word and its frequency
struct wordStat {
    char name[WORD_MAX];
    int  count;
};
typedef struct wordStat WordStat;

// Everything the analyzer owns. All heap memory hangs off this struct.
typedef struct Analyzer {
    char     *text;            
    size_t    text_len;        
    size_t    text_cap;        
    WordStat *stats;           
    int       distinct;        
    int       capacity;        
    long      total_words;     
    int       truncated_words; 
    int       selected;        
    int       running;         
    char      source[SOURCE_MAX];
} Analyzer;

// Every menu operation has this signature so it fits in one array.
typedef void (*Operation)(Analyzer *);
// Function
void read_line(const char *prompt, char *buf, size_t size);
int  read_int(const char *prompt, int *out);
void text_reset(Analyzer *a);
int  text_append(Analyzer *a, const char *chunk, size_t n);
void load_text_from_file(Analyzer *a);
void read_pasted_text(Analyzer *a);
const char *scan_word(const char *p, char *word, int *truncated);
int  analyze_text(Analyzer *a);
void count_words(Analyzer *a);
void longest_word(Analyzer *a);
void most_frequent(Analyzer *a);
void display_word_statistics(Analyzer *a);
void sort_word_statistics(Analyzer *a);
void search_word(Analyzer *a);
void generate_text_summary(Analyzer *a);
void terminate_program(Analyzer *a);
double average_word_length(const Analyzer *a);
double lexical_diversity(const Analyzer *a);
const char *diversity_label(double score);
void custom_analysis(Analyzer *a);
void memory_inspection(Analyzer *a);

#endif /* MAIN_H */