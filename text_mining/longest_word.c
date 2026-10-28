#include "main.h"

// identifying the loongest word in the paragraph
void find_longest_word(WordStats *stats, int word_count) {
    if (word_count <= 0) {
        printf("No words to analyze.\n");
        return;
    }
    int longest_index = 0;
    for (int i = 1; i < word_count; i++) {
        if (strlen(stats[i].name) > strlen(stats[longest_index].name))
            longest_index = i;
    }
    printf("The longest word is: '%s' with length %zu\n", stats[longest_index].name, strlen(stats[longest_index].name));
} 