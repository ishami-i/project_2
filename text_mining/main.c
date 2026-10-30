#include "main.h"

static void print_header(const Analyzer *a) {
    printf("Student ID: %s\n", STUDENT_ID_HEADER);
    printf("Project: %s\n", PROJECT_TITLE);
    printf("Source: %s\n", a->source);
    printf("Custom Analysis: %s\n", CUSTOM_ANALYSIS);
}

static void print_menu(void) {
    printf("\n----------------- MENU -----------------\n");
    printf(" 1. Load paragraph from file\n");
    printf(" 2. Enter / paste a paragraph\n");
    printf(" 3. Count words\n");
    printf(" 4. Find the longest word\n");
    printf(" 5. Find the most frequent word\n");
    printf(" 6. Display word statistics\n");
    printf(" 7. Sort word statistics\n");
    printf(" 8. Search for a word\n");
    printf(" 9. Custom analysis (Lexical Diversity Score)\n");
    printf("10. Generate text summary\n");
    printf("11. Memory inspection\n");
    printf("12. Exit\n");
}
void terminate_program(Analyzer *a) {
    printf("Exiting the program. Goodbye!\n");
    a->running = 0;
}
int main(void) {
    Analyzer a = {0};
    a.selected = -1;
    a.running = 1;
    strcpy(a.source, DEFAULT_SOURCE);

    // function-pointer table: menu choice N runs actions[N-1]
    Operation actions[] = {
        load_text_from_file,
        read_pasted_text,
        count_words,
        longest_word,
        most_frequent,
        display_word_statistics,
        sort_word_statistics,
        search_word,
        custom_analysis,
        generate_text_summary,
        memory_inspection,
        terminate_program,
    };
    int num_actions = (int)(sizeof(actions) / sizeof(actions[0]));

    print_header(&a);

    // load the default paragraph so the menu works immediately
    FILE *fp = fopen(PARAGRAPH_FILE, "r");
    if (fp != NULL) {
        char chunk[128];
        size_t n;
        int ok = 1;
        while (ok && (n = fread(chunk, 1, sizeof(chunk), fp)) > 0)
            ok = text_append(&a, chunk, n);
        fclose(fp);
        if (ok && analyze_text(&a))
            printf("Loaded %s: %ld words, %d distinct.\n", PARAGRAPH_FILE, a.total_words, a.distinct);
    } else {
        printf("Could not open %s - use option 2 to paste text.\n", PARAGRAPH_FILE);
    }

    while (a.running) {
        int choice;
        print_menu();
        if (!read_int("Enter your choice (1-12): ", &choice)) {
            if (feof(stdin)) break;
            printf("Invalid input. Please enter a number between 1 and %d.\n", num_actions);
            continue;
        }
        if (choice < 1 || choice > num_actions) {
            printf("Invalid choice. Please enter a number between 1 and %d.\n", num_actions);
            continue;
        }
        actions[choice - 1](&a);        
    }

    text_reset(&a);                      
    return 0;
}
