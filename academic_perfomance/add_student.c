#include "main.h"

// Allocate the initial block for 15 students (calloc zero-fills it).
int allocate_students(Database *db) {
    db->data = (Student *)calloc(INITIAL_CAPACITY, sizeof(Student));
    if (db->data == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    db->count = 0;
    db->capacity = INITIAL_CAPACITY;
    db->running = 1;
    return 1;
}

int id_exists(const Database *db, int id) {
    for (const Student *p = db->data; p < db->data + db->count; p++)
        if (p->id == id) return 1;
    return 0;
}

// Load the starting dataset from students.txt
int load_students(Database *db) {
    FILE *fp = fopen(STUDENTS_FILE, "r");
    char buffer[300];
    if (fp == NULL) {
        printf("Error opening %s!\n", STUDENTS_FILE);
        return 0;
    }
    while (db->count < INITIAL_CAPACITY && fgets(buffer, sizeof(buffer), fp) != NULL) {
        Student s;
        if (sscanf(buffer, "%d \"%99[^\"]\" \"%49[^\"]\" \"%f\"",
                   &s.id, s.name, s.course, &s.grade) != 4) {
            continue;                                   
        }
        if (s.grade < 0 || s.grade > 100 || s.id <= 0 || id_exists(db, s.id))
            continue;                                  
        db->data[db->count++] = s;
    }
    fclose(fp);
    return db->count;
}

// Add a new student: validate input, reject duplicate IDs, grow with realloc
void add_student(Database *db) {
    Student s;

    if (!read_int("Enter student ID (positive number): ", &s.id) || s.id <= 0) {
        printf("Invalid ID.\n");
        return;
    }
    if (id_exists(db, s.id)) {
        printf("Error: a student with ID %d already exists.\n", s.id);
        return;
    }
    read_line("Enter student name: ", s.name, sizeof(s.name));
    read_line("Enter course: ", s.course, sizeof(s.course));
    if (!read_float("Enter grade (0-100): ", &s.grade) || s.grade < 0 || s.grade > 100) {
        printf("Invalid grade. It must be a number from 0 to 100.\n");
        return;
    }

    if (db->count == db->capacity) {
        int new_cap = db->capacity * 2;
        Student *tmp = (Student *)realloc(db->data, new_cap * sizeof(Student));
        if (tmp == NULL) {                            
            printf("Memory allocation failed! Student not added.\n");
            return;
        }
        db->data = tmp;
        printf("[memory] capacity grew from %d to %d records\n", db->capacity, new_cap);
        db->capacity = new_cap;
    }
    db->data[db->count++] = s;
    printf("Student added successfully! Total records: %d\n", db->count);
}