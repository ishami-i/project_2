#include "main.h"

// Remove a record by ID, close the gap, and shrink memory when it is mostly empty.
void remove_student(Database *db) {
    int id;
    if (db->count == 0) { printf("No records to remove.\n"); return; }

    display_students(db);
    if (!read_int("Enter the ID of the student to remove: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    for (int i = 0; i < db->count; i++) {
        if (db->data[i].id != id) continue;

        // shift everything after index i one place to the left
        memmove(&db->data[i], &db->data[i + 1], (db->count - i - 1) * sizeof(Student));
        db->count--;
        printf("Student with ID %d removed successfully. Total records: %d\n", id, db->count);

        // shrink the block to half when fewer than half of the slots are used
        if (db->capacity > 1 && db->count < db->capacity / 2) {
            int new_cap = db->capacity / 2;
            if (new_cap < db->count) new_cap = db->count;
            if (new_cap < 1) new_cap = 1;
            Student *tmp = (Student *)realloc(db->data, new_cap * sizeof(Student));
            if (tmp != NULL) {
                db->data = tmp;
                printf("[memory] capacity shrank from %d to %d records\n", db->capacity, new_cap);
                db->capacity = new_cap;
            }
        }
        return;
    }
    printf("Student with ID %d not found.\n", id);
}