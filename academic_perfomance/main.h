#ifndef MAIN_H
#define MAIN_H

// library includes
#include <stdio.h>
#include <stdlib.h>

// student structure definition
typedef struct Student {
    int id;
    char name[100];
    char course[50];
    float grade;
} Student;


// functions declared int this directory
void terminate_program();
int add_student();
void display_students(Student *students, int count);
int insert_students(Student *students);
void allocate_students(Student **students);
float calculate_average_grade(Student *students, int count);

#endif // MAIN_H