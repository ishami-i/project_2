#include "main.h"

static void print_student(const Student *s) {
    printf("Student found:\n  ID: %d\n  Name: %s\n  Course: %s\n  Grade: %.2f\n",
           s->id, s->name, s->course, s->grade);
}

// Search by ID or by exact name (strcmp)
void search_student(Database *db) {
    int choice;
    if (db->count == 0) { printf("No records to search.\n"); return; }

    printf("Search by:\n  1. Student ID\n  2. Student Name\n");
    if (!read_int("Enter choice (1 or 2): ", &choice)) { printf("Invalid input.\n"); return; }

    if (choice == 1) {
        int id;
        if (!read_int("Enter student ID: ", &id)) { printf("Invalid ID.\n"); return; }
        for (const Student *p = db->data; p < db->data + db->count; p++)
            if (p->id == id) { print_student(p); return; }
        printf("Student with ID %d not found.\n", id);
    } else if (choice == 2) {
        char name[MAX_NAME];
        read_line("Enter student name: ", name, sizeof(name));
        for (const Student *p = db->data; p < db->data + db->count; p++)
            if (strcmp(p->name, name) == 0) { print_student(p); return; }
        printf("Student \"%s\" not found.\n", name);
    } else {
        printf("Invalid choice. Please enter 1 or 2.\n");
    }
}
