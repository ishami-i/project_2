#include "main.h"

// function for displaying the word statistics
void display_word_statistics(WordStats *stats, int word_count) {
    if (word_count <= 0) {
        printf("No words to display.\n");
        return;
    }
    printf("Word Statistics:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("-------------------- -----\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", stats[i].name, stats[i].count);
    }
}