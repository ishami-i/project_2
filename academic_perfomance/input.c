#include "main.h"

// Reads one line safely and strips the newline
void read_line(const char *prompt, char *buf, size_t size) {
    for (;;) {
        printf("%s", prompt);
        if (fgets(buf, (int)size, stdin) == NULL) {
            buf[0] = '\0';
            return;
        }
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        } else {                                       
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
        if (buf[0] != '\0') return;                     
        printf("Input cannot be empty.\n");
    }
}

/* Returns 1 and sets *out on success, 0 on invalid input. */
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

int read_float(const char *prompt, float *out) {
    char line[64], *end;
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    float v = strtof(line, &end);
    while (*end == ' ' || *end == '\t') end++;
    if (end == line || *end != '\n') return 0;
    *out = v;
    return 1;
}
