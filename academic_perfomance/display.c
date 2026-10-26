#include "main.h"

// function to display all student records in a formatted table
void display_students(Student *students, int count) {
    printf("ID\tName\tCourse\tGrade\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%s\t%.2f\n", students[i].id, students[i].name, students[i].course, students[i].grade);
    }
}