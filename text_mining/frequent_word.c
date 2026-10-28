#include "main.h"

// function to count then display the most frequent word in the paragraph
void most_frequent_word(WordStats *stats, int word_count) {
    if (word_count <= 0) {
        printf("No words to analyze.\n");
        return;
    }
    int max_index = 0;
    for (int i = 1; i < word_count; i++) {
        if (stats[i].count > stats[max_index].count) {
            max_index = i;
        }
    }
    printf("The most frequent word is: '%s' with count %d\n", stats[max_index].name, stats[max_index].count);
}