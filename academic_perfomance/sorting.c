#include "main.h"

// qsort comparators: grade high->low, name A->Z (strcmp), id low->high
static int cmp_grade(const void *a, const void *b) {
    float x = ((const Student *)a)->grade, y = ((const Student *)b)->grade;
    return (x < y) - (x > y);
}
static int cmp_name(const void *a, const void *b) {
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}
static int cmp_id(const void *a, const void *b) {
    int x = ((const Student *)a)->id, y = ((const Student *)b)->id;
    return (x > y) - (x < y);
}

void sort_students(Database *db) {
    int choice;
    int (*cmp)(const void *, const void *) = NULL;  

    if (db->count < 2) {
        printf("Not enough records to sort.\n");
        return;
    }
    printf("Sort by:\n  1. Grade (high to low)\n  2. Name (A-Z)\n  3. Student ID\n");
    if (!read_int("Enter choice (1-3): ", &choice)) { printf("Invalid input.\n"); return; }

    switch (choice) {
        case 1: cmp = cmp_grade; break;
        case 2: cmp = cmp_name;  break;
        case 3: cmp = cmp_id;    break;
        default: printf("Invalid choice.\n"); return;
    }
    qsort(db->data, db->count, sizeof(Student), cmp);
    printf("Sorted successfully.\n");
    display_students(db);
}