#include "main.h"

// Average word length over ALL words (each distinct word weighted by count).
double average_word_length(const Analyzer *a) {
    if (a->total_words == 0) return 0.0;
    long letters = 0;
    for (const WordStat *s = a->stats; s < a->stats + a->distinct; s++)
        letters += (long)strlen(s->name) * s->count;
    return (double)letters / (double)a->total_words;
}

// Lexical Diversity Score = distinct words / total words  (range 0..1).
double lexical_diversity(const Analyzer *a) {
    if (a->total_words == 0) return 0.0;
    return (double)a->distinct / (double)a->total_words;
}

const char *diversity_label(double score) {
    if (score >= 0.70) return "high diversity";
    if (score >= 0.40) return "moderate diversity";
    return "low diversity / repetitive";
}

/*  
CUSTOM ANALYSIS: LEXICAL DIVERSITY SCORE
    Formula : LDS = distinct words / total words
    Extra   : hapax legomena = words that occur exactly once;
              hapax ratio    = hapax / distinct words.
    Levels  : LDS >= 0.70 high, 0.40 - 0.69 moderate, < 0.40 low.
    Note    : LDS falls as texts get longer (common words repeat), so only
              compare scores of passages of similar length. 
*/
void custom_analysis(Analyzer *a) {
    if (a->stats == NULL || a->total_words == 0) {
        printf("No text analysed yet. Use option 1 (file) or 2 (paste) first.\n");
        return;
    }
    int hapax = 0, repeated_words = 0;
    for (const WordStat *s = a->stats; s < a->stats + a->distinct; s++) {
        if (s->count == 1) hapax++;
        else repeated_words += s->count;
    }
    double lds = lexical_diversity(a);
    printf("\n=== Lexical Diversity Score ===\n");
    printf("Purpose : measure how varied the vocabulary of the text is.\n");
    printf("Formula : LDS = distinct words / total words\n");
    printf("Levels  : >= 0.70 high, 0.40-0.69 moderate, < 0.40 low\n\n");
    printf("Distinct words : %d\n", a->distinct);
    printf("Total words    : %ld\n", a->total_words);
    printf("LDS            : %d / %ld = %.3f\n", a->distinct, a->total_words, lds);
    printf("Hapax words    : %d (%.1f%% of distinct words occur exactly once)\n",
           hapax, 100.0 * hapax / a->distinct);
    printf("Repeated usage : %d of %ld word occurrences (%.1f%%) belong to words used more than once\n",
           repeated_words, a->total_words, 100.0 * repeated_words / a->total_words);
    printf("Interpretation : %s. ", diversity_label(lds));
    if (lds >= 0.70)      printf("The author rarely repeats words.\n");
    else if (lds >= 0.40) printf("A normal mix of new and repeated words.\n");
    else                  printf("Many words are repeated; the text is repetitive or long.\n");
}

// Prints the addresses of the key heap objects (format %p, cast to void *).
void memory_inspection(Analyzer *a) {
    printf("\nMemory Inspection\n--------------------------\n");
    printf("Text buffer address       : %p\n", (void *)a->text);
    printf("Word statistics array     : %p\n", (void *)a->stats);
    if (a->stats != NULL && a->distinct > 0) {
        int i = (a->selected >= 0 && a->selected < a->distinct) ? a->selected : 0;
        printf("Selected word-stat entry  : %p  (entry %d = '%s', count %d)\n",
               (void *)(a->stats + i), i, a->stats[i].name, a->stats[i].count);
        printf("Entry size                : %zu bytes (name[%d] + int count)\n",
               sizeof(WordStat), WORD_MAX);
    } else {
        printf("Selected word-stat entry  : (none - no words stored)\n");
    }
    printf("\nMeaning of each address:\n");
    printf("  Text buffer   - start of the heap block (malloc/realloc) holding the paragraph;\n");
    printf("                  %zu of %zu bytes used.\n", a->text_len, a->text_cap);
    printf("  Stats array   - start of the heap block holding all distinct words;\n");
    printf("                  %d of %d entries used.\n", a->distinct, a->capacity);
    printf("  Selected entry- address of one WordStat inside that array (the word last\n");
    printf("                  searched for or found as most frequent, else the first one).\n");
    printf("Addresses change between runs, and when a buffer grows via realloc.\n");
}
