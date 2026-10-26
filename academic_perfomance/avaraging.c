#include "main.h"

// calculating the avarage grade of the students
float calculate_average_grade(Student *students) {
    // calculating the size of the students array
    int count = 0;
    while (students[count].id != 0) {
        count++;
    }
    if (count == 0) {
        printf("No students available to calculate average grade.\n");
        return 0.0f; // Return 0 if there are no students
    }
    for(int i = 0; i < count; i++) {
        if (students[i].grade < 0 || students[i].grade > 100) {
            printf("Invalid grade for student ID %d. Grade should be between 0 and 100.\n", students[i].id);
            return -1.0f; // Return -1 to indicate an error
        }
    }
    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        total += students[i].grade;
    }
    return total / count;
}

// avaraging function per course using prefix sum and count then return the avarage grade for the every course
float calculate_average_grade_per_course(Student *students) {
    // calculating the size of the students array
    int count = 0;
    while (students[count].id != 0) {
        count++;
    }
    // check if there are no students to avoid division by zero
    if (count == 0) {
        printf("No students available to calculate average grade.\n");
        return 0.0f; // Return 0 if there are no students
    }
    // array of courses found in the students array
    
}