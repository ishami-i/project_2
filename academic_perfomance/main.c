#include "main.h"

void terminate_program(Database *db) {
    printf("Exiting the program. Goodbye!\n");
    db->running = 0;
}

static void print_header(void) {
    printf("Student ID: %d\n", STUDENT_ID_HEADER);
    printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");
}

static void print_menu(void) {
    printf("\n----------------- MENU -----------------\n");
    printf(" 1. Add a new student record\n");
    printf(" 2. Display all student records\n");
    printf(" 3. Sort students (grade / name / ID)\n");
    printf(" 4. Calculate the average grade\n");
    printf(" 5. Identify the top-performing student\n");
    printf(" 6. Search for a student (ID or name)\n");
    printf(" 7. Remove a student record\n");
    printf(" 8. Run custom analysis (Academic Risk Index)\n");
    printf(" 9. Display summary report\n");
    printf("10. Exit\n");
}

int main(void) {
    Database db = {NULL, 0, 0, 1};

    // Function-pointer table
    MenuFunction actions[10] = {
        add_student,
        display_students,
        sort_students,
        calculate_average,
        find_top_student,
        search_student,
        remove_student,
        custom_analysis,
        generate_summary_report,
        terminate_program        
    };

    print_header();

    if (!allocate_students(&db)) return 1;
    if (!load_students(&db)) {
        free(db.data);
        return 1;
    }
    printf("Loaded %d students from %s.\n", db.count, STUDENTS_FILE);

    while (db.running) {
        int choice;
        print_menu();
        if (!read_int("Enter your choice (1-10): ", &choice)) {
            if (feof(stdin)) break;
            printf("Invalid input. Please enter a number between 1 and 10.\n");
            continue;
        }
        if (choice < 1 || choice > 10) {
            printf("Invalid choice. Please enter a number between 1 and 10.\n");
            continue;
        }
        // function pointer
        actions[choice - 1](&db);
    }

    // release all heap memory 
    free(db.data);                            
    db.data = NULL;
    return 0;
}
