#ifndef MAIN_H
#define MAIN_H

// Library included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// header contents
#define STUDENT_ID_HEADER  "00001"
#define PARAGRAPH_FILE      "paragraph.txt"
#define BOOK_TITLE_FILE      "Harry Potter and the Chamber of Secrets"


// data structure
typedef struct Text {
    char name[40];
    int count;
} WordStats;

// the functions used 
int count_words(WordStats *stats, int word_count);
int read_paragraph(WordStats *stats, int max_words);
void find_longest_word(WordStats *stats, int word_count);
void find_longest_word(WordStats *stats, int word_count);
void most_frequent_word(WordStats *stats, int word_count);
void sort_words(WordStats *stats, int word_count, int (*compare)(const WordStats *, const WordStats *));
void display_word_statistics(WordStats *stats, int word_count);
void generate_summary_report(WordStats *stats, int word_count);
void search_word(WordStats *stats, int word_count, const char *word);
void avarage_word_length(WordStats *stats, int word_count);


#endif // MAIN_H