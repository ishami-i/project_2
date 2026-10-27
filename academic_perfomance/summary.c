#include "main.h"

void generate_summary_report(Database *db) {
    if (db->count == 0) {
        printf("No students available to summarise.\n");
        return;
    }
    const Student *end = db->data + db->count;
    const Student *hi = db->data, *lo = db->data;
    int pass = 0;

    for (const Student *p = db->data; p < end; p++) {
        if (p->grade > hi->grade) hi = p;
        if (p->grade < lo->grade) lo = p;
        if (p->grade >= PASS_THRESHOLD) pass++;
    }

    printf("\n========== SUMMARY REPORT ==========\n");
    printf("Total student records : %d\n", db->count);
    printf("Average grade         : %.2f\n", compute_average(db));
    printf("Highest grade         : %.2f (%s)\n", hi->grade, hi->name);
    printf("Lowest grade          : %.2f (%s)\n", lo->grade, lo->name);
    printf("Pass threshold        : %.0f\n", PASS_THRESHOLD);
    printf("Students passing      : %d\n", pass);
    printf("Students below        : %d\n\n", db->count - pass);
    print_course_averages(db);
    printf("\n");
    print_risk_summary(db);
    printf("====================================\n");
}
