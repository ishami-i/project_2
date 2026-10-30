#include "main.h"

// Returns 1 if there is analysed text, otherwise prints a hint and returns 0.
static int has_data(const Analyzer *a) {
    if (a->stats == NULL || a->total_words == 0) {
        printf("No text analysed yet. Use option 1 (file) or 2 (paste) first.\n");
        return 0;
    }
    return 1;
}

// count_words()
void count_words(Analyzer *a) {
    if (!has_data(a)) return;
    long total = 0;
    for (const WordStat *s = a->stats; s < a->stats + a->distinct; s++)
        total += s->count;
    printf("Total number of words   : %ld\n", total);
    printf("Number of distinct words: %d\n", a->distinct);
    if (a->truncated_words > 0)
        printf("Note: %d word(s) longer than 39 characters were truncated.\n",
               a->truncated_words);
}

// longest_word()
void longest_word(Analyzer *a) {
    if (!has_data(a)) return;
    const WordStat *best = a->stats;
    for (const WordStat *s = a->stats + 1; s < a->stats + a->distinct; s++)
        if (strlen(s->name) > strlen(best->name)) best = s;
    printf("The longest word is '%s' with length %zu\n", best->name, strlen(best->name));
}

// most_frequent()
void most_frequent(Analyzer *a) {
    if (!has_data(a)) return;
    const WordStat *best = a->stats;
    for (const WordStat *s = a->stats + 1; s < a->stats + a->distinct; s++)
        if (s->count > best->count) best = s;
    int ties = 0;
    for (const WordStat *s = a->stats; s < a->stats + a->distinct; s++)
        if (s->count == best->count) ties++;
    printf("The most frequent word is '%s' with count %d", best->name, best->count);
    if (ties > 1) printf(" (%d words share this count)", ties);
    printf("\n");
    a->selected = (int)(best - a->stats);
}

// display_word_statistics(): table of every distinct word.
void display_word_statistics(Analyzer *a) {
    if (!has_data(a)) return;
    printf("\n%-24s %s\n", "Word", "Count");
    printf("------------------------ -----\n");
    for (const WordStat *s = a->stats; s < a->stats + a->distinct; s++)
        printf("%-24s %d\n", s->name, s->count);
    printf("------------------------ -----\n");
    printf("%d distinct words, %ld total\n", a->distinct, a->total_words);
}

// qsort comparators: chosen through a function pointer
static int cmp_alpha(const void *x, const void *y) {
    return strcmp(((const WordStat *)x)->name, ((const WordStat *)y)->name);
}
static int cmp_freq(const void *x, const void *y) {
    const WordStat *p = x, *q = y;
    // high to low
    if (p->count != q->count) return q->count - p->count;   
    return strcmp(p->name, q->name);
}

// sort_word_statistics(): alphabetically or by frequency.
void sort_word_statistics(Analyzer *a) {
    int choice;
    int (*cmp)(const void *, const void *);
    if (!has_data(a)) return;
    printf("Sort by:\n  1. Alphabetical (A-Z)\n  2. Frequency (high to low)\n");
    if (!read_int("Enter choice (1-2): ", &choice)) { printf("Invalid input.\n"); return; }
    if (choice == 1) cmp = cmp_alpha;
    else if (choice == 2) cmp = cmp_freq;
    else { printf("Invalid choice.\n"); return; }
    qsort(a->stats, (size_t)a->distinct, sizeof(WordStat), cmp);
    a->selected = -1;                      
    printf("Sorted successfully.\n");
    display_word_statistics(a);
}

// search_word(): the query is normalized with the same rules as the text.
void search_word(Analyzer *a) {
    char input[256], word[WORD_MAX];
    int truncated;
    if (!has_data(a)) return;
    read_line("Enter word to search: ", input, sizeof(input));
    if (scan_word(input, word, &truncated) == NULL) { printf("No word entered.\n"); return; }
    for (WordStat *s = a->stats; s < a->stats + a->distinct; s++) {
        if (strcmp(s->name, word) == 0) {
            printf("The word '%s' appears %d time(s).\n", word, s->count);
            a->selected = (int)(s - a->stats);
            return;
        }
    }
    printf("The word '%s' does not appear in the text.\n", word);
}

// generate_text_summary(): overall statistics, including the custom analysis.
void generate_text_summary(Analyzer *a) {
    if (!has_data(a)) return;
    const WordStat *longest = a->stats, *freq = a->stats;
    for (const WordStat *s = a->stats + 1; s < a->stats + a->distinct; s++) {
        if (strlen(s->name) > strlen(longest->name)) longest = s;
        if (s->count > freq->count) freq = s;
    }
    double lds = lexical_diversity(a);
    printf("\n============ TEXT SUMMARY ============\n");
    printf("Source                : %s\n", a->source);
    printf("Total number of words : %ld\n", a->total_words);
    printf("Distinct words        : %d\n", a->distinct);
    printf("Average word length   : %.2f characters\n", average_word_length(a));
    printf("Longest word          : '%s' (%zu characters)\n", longest->name, strlen(longest->name));
    printf("Most frequent word    : '%s' (%d times)\n", freq->name, freq->count);
    printf("Lexical Diversity     : %.3f (%s)\n", lds, diversity_label(lds));
    if (a->truncated_words > 0)
        printf("Truncated words       : %d\n", a->truncated_words);
    printf("======================================\n");
}

void terminate_prorram(Analyzer *a) {
    printf("Exiting the program. Goodbye!\n");
    a->running = 0;
}