#include "main.h"

void print_header(void) {
    printf("Student ID: %s\n", STUDENT_ID_HEADER);
    printf("Project: Text Mining and Word Statistics Analysis\n");
    printf("Source: %s\n", BOOK_TITLE_FILE);
    printf("Custom Analysis: Average Word Length\n");
}

void terminate_program(void) {
    printf("Exiting the program. Goodbye!\n");
    exit(0);
}
void menu(void) {
    printf("\n----------------- MENU -----------------\n");
    printf(" 1. Read paragraph from file\n");
    printf(" 2. Display word statistics\n");
    printf(" 3. Sort words (alphabetically / frequency)\n");
    printf(" 4. Search for a word\n");
    printf(" 5. Generate summary report\n");
    printf(" 6. Calculate average word length\n");
    printf(" 7. Exit\n");
}

int main(void) {
    WordStats stats[1000]; // Assuming a maximum of 1000 unique words
    int word_count = 0;

    print_header();

    while (1) {
        menu();
        int choice;
        printf("Enter your choice (1-7): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 7.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        // function pointer array for menu options
        void (*actions[])(WordStats *, int *) = {
            read_paragraph,
            display_word_statistics,
            sort_words,
            search_word,
            generate_summary_report,
            avarage_word_length,
            terminate_program,
        };
        for(int i = 0; i < sizeof(actions) / sizeof(actions[0]); i++) {
            if (choice == i + 1) {
                actions[i](stats, &word_count);
                break;
            }
        }

    }

    return 0;
}