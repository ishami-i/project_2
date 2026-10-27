#include "main.h"

// Find the highest grade with pointer traversal, then print every student with it.
void find_top_student(Database *db) {
    if (db->count == 0) {
        printf("No students available.\n");
        return;
    }
    const Student *end = db->data + db->count;
    float best = db->data->grade;
    for (const Student *p = db->data + 1; p < end; p++)
        if (p->grade > best) best = p->grade;

    printf("Top-performing student(s):\n");
    for (const Student *p = db->data; p < end; p++)
        if (p->grade == best)
            printf("  ID: %d | Name: %s | Course: %s | Grade: %.2f\n",
                   p->id, p->name, p->course, p->grade);
}