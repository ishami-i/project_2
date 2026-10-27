#include "main.h"

// Average of all grades, using a moving pointer. Returns 0 if no records.
float compute_average(const Database *db) {
    if (db->count == 0) return 0.0f;
    float total = 0.0f;
    for (const Student *p = db->data; p < db->data + db->count; p++)
        total += p->grade;
    return total / db->count;
}

void calculate_average(Database *db) {
    if (db->count == 0) {
        printf("No students available to calculate an average.\n");
        return;
    }
    printf("Average grade of %d students: %.2f\n", db->count, compute_average(db));
    print_course_averages(db);
}

// Average per course
void print_course_averages(const Database *db) {
    if (db->count == 0) return;

    const char **courses = (const char **)malloc(db->count * sizeof(char *));
    if (courses == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    int n = 0;
    for (const Student *p = db->data; p < db->data + db->count; p++) {
        int found = 0;
        for (int j = 0; j < n; j++)
            if (strcmp(p->course, courses[j]) == 0) { found = 1; break; }
        if (!found) courses[n++] = p->course;
    }

    printf("Average grade per course:\n");
    for (int j = 0; j < n; j++) {
        float total = 0.0f;
        int cnt = 0;
        for (const Student *p = db->data; p < db->data + db->count; p++)
            if (strcmp(p->course, courses[j]) == 0) { total += p->grade; cnt++; }
        printf("  %-24s %6.2f  (%d student%s)\n", courses[j], total / cnt, cnt, cnt == 1 ? "" : "s");
    }
    free(courses);
}
