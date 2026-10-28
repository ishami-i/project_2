#include "main.h"

// calculates the average number of characters per word
void avarage_word_length(WordStats *stats, int word_count) {
    if (word_count <= 0) {
        printf("No words to analyze.\n");
        return;
    }
    int total_length = 0;
    for (int i = 0; i < word_count; i++) {
        total_length += strlen(stats[i].name);
    }
    float average_length = (float)total_length / word_count;
    printf("Average word length: %.2f characters\n", average_length);
}