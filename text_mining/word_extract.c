#include "main.h"

/*  TEXT-PROCESSING RULES (documented as required)
    - A word is a run of letters/digits.
    - Case:        converted to lowercase ("The" and "the" are one word).
    - Punctuation: any other character is a separator and is dropped
                   (. , ; : ! ? " ( ) and the em dash).
    - Apostrophes: kept INSIDE a word ("don't", "Alice's") and stored as '.
                   The curly apostrophe (UTF-8 bytes E2 80 99) is treated the
                   same. Leading/trailing apostrophes (quotes) are dropped.
    - Hyphens:     kept INSIDE a word ("rabbit-hole" is one word); leading,
                   trailing and double hyphens are separators.
    - Whitespace:  any amount of spaces/tabs/newlines separates words.
    - Length:      words longer than 39 characters are TRUNCATED to 39
                   characters (counted in truncated_words and reported). */

static int apostrophe_len(const char *p) {
    if (*p == '\'') return 1;
    if ((unsigned char)p[0] == 0xE2 && (unsigned char)p[1] == 0x80 &&
        (unsigned char)p[2] == 0x99) return 3;
    return 0;
}

/* length of a joining character (hyphen / apostrophe) at p, 0 if none */
static int joiner_len(const char *p) {
    if (*p == '-') return 1;
    return apostrophe_len(p);
}

/* Finds the next word at or after p using pointer traversal only.
   Copies it, lowercased, into word[WORD_MAX]. Returns the pointer just past
   the word, or NULL when no more words exist. */
const char *scan_word(const char *p, char *word, int *truncated) {
    size_t len = 0;
    *truncated = 0;

    while (*p != '\0' && !isalnum((unsigned char)*p)) p++;   /* skip separators */
    if (*p == '\0') return NULL;

    while (*p != '\0') {
        char c;
        if (isalnum((unsigned char)*p)) {
            c = (char)tolower((unsigned char)*p);
            p++;
        } else {
            int jl = joiner_len(p);
            if (jl == 0 || !isalnum((unsigned char)p[jl])) break;  /* word ends */
            c = (*p == '-') ? '-' : '\'';
            p += jl;
        }
        if (len < WORD_MAX - 1) word[len++] = c;
        else *truncated = 1;
    }
    word[len] = '\0';
    return p;
}

/* Adds one word: counts a duplicate or appends a new entry (growing the array). */
static int add_word(Analyzer *a, const char *word) {
    for (WordStat *s = a->stats; s < a->stats + a->distinct; s++) {
        if (strcmp(s->name, word) == 0) { s->count++; return 1; }
    }
    if (a->distinct == a->capacity) {
        int new_cap = a->capacity * 2;
        WordStat *tmp = (WordStat *)realloc(a->stats, (size_t)new_cap * sizeof(WordStat));
        if (tmp == NULL) { printf("Memory allocation failed!\n"); return 0; }
        printf("[memory] word-statistics array grew from %d to %d entries\n",
               a->capacity, new_cap);
        a->stats = tmp;
        a->capacity = new_cap;
    }
    WordStat *slot = a->stats + a->distinct;
    strcpy(slot->name, word);            /* safe: word is at most 39 chars */
    slot->count = 1;
    a->distinct++;
    return 1;
}

/* Builds the word statistics from a->text. Returns 1 if at least one word. */
int analyze_text(Analyzer *a) {
    free(a->stats);
    a->stats = NULL; a->distinct = 0; a->capacity = 0;
    a->total_words = 0; a->truncated_words = 0; a->selected = -1;

    a->stats = (WordStat *)calloc(INITIAL_STATS_CAP, sizeof(WordStat));
    if (a->stats == NULL) { printf("Memory allocation failed!\n"); return 0; }
    a->capacity = INITIAL_STATS_CAP;

    if (a->text == NULL || a->text[0] == '\0') {
        printf("Input is empty. Nothing to analyze.\n");
        return 0;
    }

    char word[WORD_MAX];
    int truncated;
    const char *p = a->text;
    while ((p = scan_word(p, word, &truncated)) != NULL) {
        if (truncated) a->truncated_words++;
        if (!add_word(a, word)) return 0;
        a->total_words++;
    }
    if (a->total_words == 0) {
        printf("The text contains no words (only punctuation/whitespace).\n");
        return 0;
    }
    return 1;
}