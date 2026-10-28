#ifndef MAIN_H
#define MAIN_H

// Library included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// header contents
#define STUDENT_ID_HEADER  "00001"
#define PARAGRAPH_FILE      "paragraph.txt"
#define BOOK_TITLE_FILE      "Harry Potter and the Chamber of Secrets"


// data structure
typedef struct Text {
    char name[40];
    int count;
} WordStats;


#endif // MAIN_H