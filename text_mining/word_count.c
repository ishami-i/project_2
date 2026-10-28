#include "main.h"

// function for counting words in a structure
int count_words(WordStats *stats, int word_count) {
    int total_count = 0;
    for (int i = 0; i < word_count; i++) {
        total_count += stats[i].count;
    }
    return total_count;
}