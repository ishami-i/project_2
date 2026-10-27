#include "main.h"

/*
   CUSTOM ANALYSIS: ACADEMIC RISK INDEX
   Purpose : give every student a 0-100 score showing how close they are to
             (or how far below) a comfortable academic level.
   Formula : risk = 0                                  if grade >= SAFE (70)
             risk = (SAFE - grade) / SAFE * 100        if grade <  SAFE
   Levels  : HIGH     grade <  PASS (60)  -> failing, needs intervention
             MODERATE PASS <= grade < SAFE -> passing but borderline
             LOW      grade >= SAFE (70)  -> no concern
*/
float risk_index(float grade) {
    if (grade >= SAFE_THRESHOLD) return 0.0f;
    return (SAFE_THRESHOLD - grade) / SAFE_THRESHOLD * 100.0f;
}

static const char *risk_level(float grade) {
    if (grade < PASS_THRESHOLD) return "HIGH";
    if (grade < SAFE_THRESHOLD) return "MODERATE";
    return "LOW";
}

// Compact version used by the summary report.
void print_risk_summary(const Database *db) {
    int low = 0, moderate = 0, high = 0;
    float total_risk = 0.0f, max_risk = -1.0f;
    const Student *worst = NULL;

    for (const Student *p = db->data; p < db->data + db->count; p++) {
        float r = risk_index(p->grade);
        total_risk += r;
        if (r > max_risk) { max_risk = r; worst = p; }
        if (p->grade < PASS_THRESHOLD) high++;
        else if (p->grade < SAFE_THRESHOLD) moderate++;
        else low++;
    }
    printf("Academic Risk Index (SAFE=%.0f, PASS=%.0f):\n", SAFE_THRESHOLD, PASS_THRESHOLD);
    printf("  Average risk index : %.2f\n", total_risk / db->count);
    printf("  LOW / MODERATE / HIGH risk students: %d / %d / %d\n", low, moderate, high);
    if (worst != NULL && max_risk > 0)
        printf("  Highest risk       : %s (ID %d, grade %.2f, index %.2f)\n",
               worst->name, worst->id, worst->grade, max_risk);
    else
        printf("  Highest risk       : none - every student is at or above %.0f\n", SAFE_THRESHOLD);
}

// Full analysis shown from menu option 8.
void custom_analysis(Database *db) {
    if (db->count == 0) {
        printf("No students available for analysis.\n");
        return;
    }
    printf("\n=== Academic Risk Index ===\n");
    printf("Purpose : find students who are failing or borderline.\n");
    printf("Formula : risk = (%.0f - grade) / %.0f * 100 when grade < %.0f, otherwise 0\n",
           SAFE_THRESHOLD, SAFE_THRESHOLD, SAFE_THRESHOLD);
    printf("Levels  : HIGH (< %.0f), MODERATE (%.0f-%.0f), LOW (>= %.0f)\n\n",
           PASS_THRESHOLD, PASS_THRESHOLD, SAFE_THRESHOLD - 0.01f, SAFE_THRESHOLD);

    printf("%-6s %-22s %6s %8s  %s\n", "ID", "Name", "Grade", "Risk", "Level");
    printf("---------------------------------------------------------\n");
    for (const Student *p = db->data; p < db->data + db->count; p++)
        printf("%-6d %-22.22s %6.2f %8.2f  %s\n",
               p->id, p->name, p->grade, risk_index(p->grade), risk_level(p->grade));
    printf("---------------------------------------------------------\n");
    print_risk_summary(db);
    printf("Interpretation: 0 = safe; closer to 100 = more urgent. Any HIGH student\n"
           "should be contacted first, MODERATE students should be monitored.\n");
}
