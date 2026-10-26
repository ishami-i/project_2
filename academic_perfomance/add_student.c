#include "main.h"

// function for dynamically allocate memory for a 15 students
void allocate_students(Student **students) {
    *students = (Student *)malloc(15 * sizeof(Student));
    if (*students == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
}
// inserting the 15 students into the system from students.txt file
int insert_students(Student *students) {
    FILE *filePointer;
    char buffer[200];
    int i = 0;

    filePointer = fopen("students.txt", "r");
    if (filePointer == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL && i < 15) {
        sscanf(buffer, "%d %s %s %f", &students[i].id, students[i].name, students[i].course, &students[i].grade);
        i++;
    }
    fclose(filePointer);
    return i; // Return the number of students inserted
}

// Function to add a new student to the system from user input
int add_student() {
    Student new_student;
    printf("Enter student ID: ");
    scanf("%d", &new_student.id);
    printf("Enter student name: ");
    scanf("%s", new_student.name);
    printf("Enter student course: ");
    scanf("%s", new_student.course);
    printf("Enter student grade(out of 100): ");
    scanf("%f", &new_student.grade);

    // Here you would typically add the new student to your data structure
    // For example, if you have an array of students, you would append it to that array

    printf("Student added successfully!\n");
    return 0; // Return 0 to indicate success
}