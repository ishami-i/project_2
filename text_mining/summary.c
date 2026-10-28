#include "main.h" 

// function for generating the summary report
void generate_summary_report(WordStats *stats, int word_count) {
    printf("\n--- Summary Report ---\n");
    printf("Total number of words: %d\n", &word_count);
    find_longest_word(stats, word_count);
    most_frequent_word(stats, word_count);
    printf("----------------------\n");
}