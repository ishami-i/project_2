#include "main.h"

// for calling the functions 
void terminate_program() {
    printf("Exiting the program. Goodbye!\n");
    // terminate the whole program
    exit(0);
}

static void print_menu(void) {
    printf("Academic Perfomance and Risk Analysis\n");
    printf("1. Add a new student record.\n");
    printf("2. Display all student records in a formatted table.\n"); 
    printf("3. Sort students by grade, name, or student ID.\n");
    printf("4. Calculate the average grade.\n");
    printf("5. Identify the top-performing student.\n");
    printf("6. Search for a student using their ID or name.\n");
    printf("7. Remove a student record.\n");
    printf("8. Run a custom academic analysis.\n");
    printf("9. Display a summary report.\n");
    printf("10. Exit the program.\n");
    printf("Enter your choice (1-10): ");
} 

int main() {
    int choice = 0;
    while(choice != 0) {
        // calling for the menu 
        print_menu();
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                // Add a new student record
                break;
            case 2:
                // Display all student records in a formatted table
                break;
            case 3:
                // Sort students by grade, name, or student ID
                break;
            case 4:
                // Calculate the average grade
                break;
            case 5:
                // Identify the top-performing student
                break;
            case 6:
                // Search for a student using their ID or name
                break;
            case 7:
                // Remove a student record
                break;
            case 8:
                // Run a custom academic analysis
                break;
            case 9:
                // Display a summary report
                break;
            case 10:
                terminate_program();
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 10.\n");
    }
    return 0;
}