#ifndef MAIN_H
#define MAIN_H

// Library included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project constants
#define STUDENT_ID_HEADER  "001"
#define STUDENTS_FILE      "students.txt"
#define INITIAL_CAPACITY   15 
#define MAX_NAME           100
#define MAX_COURSE         50
#define PASS_THRESHOLD     60.0f
#define SAFE_THRESHOLD     70.0f

// Data structures
typedef struct Student {
    int   id;
    char  name[MAX_NAME];
    char  course[MAX_COURSE];
    float grade;
} Student;

// The growing/shrinking dataset, passed to every operation
typedef struct Database {
    Student *data;
    int      count;
    int      capacity;  
    int      running; 
} Database;

typedef void (*MenuFunction)(Database *db);

void read_line(const char *prompt, char *buf, size_t size);
int  read_int(const char *prompt, int *out);
int  read_float(const char *prompt, float *out);
int  allocate_students(Database *db);
int  load_students(Database *db);
int  id_exists(const Database *db, int id);
void add_student(Database *db);
void display_students(Database *db);
void sort_students(Database *db);
float compute_average(const Database *db);
void  calculate_average(Database *db);
void  print_course_averages(const Database *db);
void find_top_student(Database *db);
void search_student(Database *db);
void remove_student(Database *db);
float risk_index(float grade);
void  custom_analysis(Database *db);
void  print_risk_summary(const Database *db);
void generate_summary_report(Database *db);
void terminate_program(Database *db);

#endif /* MAIN_H */