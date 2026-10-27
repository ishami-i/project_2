#include "main.h"

// Display all records in a formatted table
void display_students(Database *db) {
    if (db->count == 0) {
        printf("No student records to display.\n");
        return;
    }
    printf("\n%-6s %-22s %-24s %6s\n", "ID", "Name", "Course", "Grade");
    printf("-------------------------------------------------------------\n");
    for (const Student *p = db->data; p < db->data + db->count; p++)
        printf("%-6d %-22.22s %-24.24s %6.2f\n", p->id, p->name, p->course, p->grade);
    printf("-------------------------------------------------------------\n");
    printf("Records: %d (capacity %d)\n", db->count, db->capacity);
}