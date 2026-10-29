#include "main.h"

/* Reads one line safely (never overflows buf) and strips the newline. */
void read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL) { buf[0] = '\0'; return; }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    else { int c; while ((c = getchar()) != '\n' && c != EOF) { } }
}

int read_int(const char *prompt, int *out) {
    char line[64], *end;
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    long v = strtol(line, &end, 10);
    while (*end == ' ' || *end == '\t') end++;
    if (end == line || *end != '\n') return 0;
    *out = (int)v;
    return 1;
}

/* Frees the text and the word statistics (used before loading new text). */
void text_reset(Analyzer *a) {
    free(a->text);   a->text = NULL;  a->text_len = 0; a->text_cap = 0;
    free(a->stats);  a->stats = NULL; a->distinct = 0; a->capacity = 0;
    a->total_words = 0; a->truncated_words = 0; a->selected = -1;
}

/* Appends n bytes to the text buffer, doubling its size when full.
   Returns 1 on success, 0 if memory could not be allocated. */
int text_append(Analyzer *a, const char *chunk, size_t n) {
    if (a->text == NULL) {
        a->text = (char *)malloc(INITIAL_TEXT_CAP);
        if (a->text == NULL) { printf("Memory allocation failed!\n"); return 0; }
        a->text_cap = INITIAL_TEXT_CAP;
        a->text_len = 0;
        a->text[0] = '\0';
    }
    if (a->text_len + n + 1 > a->text_cap) {
        size_t new_cap = a->text_cap;
        while (a->text_len + n + 1 > new_cap) new_cap *= 2;
        char *tmp = (char *)realloc(a->text, new_cap);
        if (tmp == NULL) { printf("Memory allocation failed!\n"); return 0; }
        printf("[memory] text buffer grew from %zu to %zu bytes\n", a->text_cap, new_cap);
        a->text = tmp;
        a->text_cap = new_cap;
    }
    memcpy(a->text + a->text_len, chunk, n);
    a->text_len += n;
    a->text[a->text_len] = '\0';
    return 1;
}

/* Option: load the paragraph from a file (default PARAGRAPH_FILE). */
void load_text_from_file(Analyzer *a) {
    char path[256], chunk[128];
    read_line("File name (Enter for " PARAGRAPH_FILE "): ", path, sizeof(path));
    if (path[0] == '\0') strcpy(path, PARAGRAPH_FILE);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) { perror("Error opening file"); return; }

    text_reset(a);
    size_t n;
    int ok = 1;
    while (ok && (n = fread(chunk, 1, sizeof(chunk), fp)) > 0)
        ok = text_append(a, chunk, n);
    fclose(fp);
    if (!ok) { text_reset(a); return; }

    if (strcmp(path, PARAGRAPH_FILE) == 0) strcpy(a->source, DEFAULT_SOURCE);
    else snprintf(a->source, sizeof(a->source), "File: %.190s", path);
    if (analyze_text(a))
        printf("Loaded %zu characters: %ld words, %d distinct.\n",
               a->text_len, a->total_words, a->distinct);
}

/* Option: type or paste a paragraph; finish with a line containing only END. */
void read_pasted_text(Analyzer *a) {
    char line[512];
    int ok = 1;
    text_reset(a);
    printf("Paste your text. Finish with a line containing only END.\n");
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strcmp(line, "END\n") == 0 || strcmp(line, "END") == 0) break;
        if (!(ok = text_append(a, line, strlen(line)))) break;
    }
    if (!ok) { text_reset(a); return; }
    strcpy(a->source, "User-supplied text");
    if (analyze_text(a))
        printf("Read %zu characters: %ld words, %d distinct.\n",
               a->text_len, a->total_words, a->distinct);
}
