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

#endif // MAIN_H