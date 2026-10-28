#include "main.h"

// function to read the content of PARAGRAPH_FILE and insert into WordStats

int read_paragraph(WordStats *stats, int max_words) {
    FILE *file = fopen(PARAGRAPH_FILE, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char word[40];
    int count = 0;

    while (fscanf(file, "%39s", word) == 1 && count < max_words) {
        strncpy(stats[count].name, word, sizeof(stats[count].name) - 1);
        stats[count].name[sizeof(stats[count].name) - 1] = '\0'; // Ensure null-termination
        stats[count].count = 1; // Initialize count to 1 for each unique word
        count++;
    }

    fclose(file);
    return count;
}