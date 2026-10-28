#include "main.h"

// Function to search the word in the paragraph and display its count
void search_word(WordStats *stats, int word_count, const char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(stats[i].name, word) == 0) {
            printf("The word '%s' appears %d times in the paragraph.\n", word, stats[i].count);
            return;
        }
    }
    printf("The word '%s' does not appear in the paragraph.\n", word);
}
