#include "main.h"

// sort by frequency or words or alphabetically
void sort_words(WordStats *stats, int word_count, int (*compare)(const WordStats *, const WordStats *)) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (compare(&stats[j], &stats[j + 1]) > 0) {
                WordStats temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }
}