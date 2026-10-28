#include "main.h"

/*
   CUSTOM ANALYSIS: ACADEMIC RISK INDEX (GPA out of 5)
   Purpose : give every student a 0-100 score showing how far their GPA is
             below the safe level (3.5).
   Step 1  : GPA = grade * 5 / 100                (grade 0-100 -> GPA 0-5)
   Step 2  : risk = 0                              if GPA >= 3.5
             risk = (3.5 - GPA) / 3.5 * 100        if GPA <  3.5
   Levels  : AT RISK       GPA <  3.0
             CLOSE TO RISK 3.0 <= GPA < 3.5
             SAFE          GPA >= 3.5
*/
float grade_to_gpa(float grade) {
    return grade * GPA_MAX / MAX_GRADE;
}

float risk_index(float gpa) {
    if (gpa >= GPA_SAFE) return 0.0f;
    return (GPA_SAFE - gpa) / GPA_SAFE * 100.0f;
}

static const char *risk_level(float gpa) {
    if (gpa < GPA_AT_RISK) return "AT RISK";
    if (gpa < GPA_SAFE) return "CLOSE TO RISK";
    return "SAFE";
}

// Compact version used by the summary report.
void print_risk_summary(const Database *db) {
    int safe = 0, close = 0, at_risk = 0;
    float total_risk = 0.0f, max_risk = -1.0f;
    const Student *worst = NULL;

    for (const Student *p = db->data; p < db->data + db->count; p++) {
        float gpa = grade_to_gpa(p->grade);
        float r = risk_index(gpa);
        total_risk += r;
        if (r > max_risk) { max_risk = r; worst = p; }
        if (gpa < GPA_AT_RISK) at_risk++;
        else if (gpa < GPA_SAFE) close++;
        else safe++;
    }
    printf("Academic Risk Index (GPA out of %.0f: at risk < %.1f, close to risk %.1f-%.1f, safe >= %.1f):\n",
           GPA_MAX, GPA_AT_RISK, GPA_AT_RISK, GPA_SAFE, GPA_SAFE);
    printf("  Average risk index : %.2f\n", total_risk / db->count);
    printf("  SAFE / CLOSE TO RISK / AT RISK students: %d / %d / %d\n", safe, close, at_risk);
    if (worst != NULL && max_risk > 0)
        printf("  Highest risk       : %s (ID %d, grade %.2f, GPA %.2f, index %.2f)\n",
               worst->name, worst->id, worst->grade, grade_to_gpa(worst->grade), max_risk);
    else
        printf("  Highest risk       : none - every student has a GPA of %.1f or above\n", GPA_SAFE);
}

// Full analysis shown from menu option 8.
void custom_analysis(Database *db) {
    if (db->count == 0) {
        printf("No students available for analysis.\n");
        return;
    }
    printf("\n=== Academic Risk Index (GPA out of %.0f) ===\n", GPA_MAX);
    printf("Purpose : find students whose GPA is at risk or close to it.\n");
    printf("Step 1  : GPA  = grade * %.0f / %.0f\n", GPA_MAX, MAX_GRADE);
    printf("Step 2  : risk = (%.1f - GPA) / %.1f * 100 when GPA < %.1f, otherwise 0\n",
           GPA_SAFE, GPA_SAFE, GPA_SAFE);
    printf("Levels  : AT RISK (GPA < %.1f), CLOSE TO RISK (%.1f to below %.1f), SAFE (GPA >= %.1f)\n\n",
           GPA_AT_RISK, GPA_AT_RISK, GPA_SAFE, GPA_SAFE);

    printf("%-6s %-22s %6s %5s %8s  %s\n", "ID", "Name", "Grade", "GPA", "Risk", "Level");
    printf("-------------------------------------------------------------------\n");
    for (const Student *p = db->data; p < db->data + db->count; p++) {
        float gpa = grade_to_gpa(p->grade);
        printf("%-6d %-22.22s %6.2f %5.2f %8.2f  %s\n",
               p->id, p->name, p->grade, gpa, risk_index(gpa), risk_level(gpa));
    }
    printf("-------------------------------------------------------------------\n");
    print_risk_summary(db);
    printf("Interpretation: 0 = safe; closer to 100 = more urgent. AT RISK students\n"
           "should be contacted first, CLOSE TO RISK students should be monitored.\n");
}