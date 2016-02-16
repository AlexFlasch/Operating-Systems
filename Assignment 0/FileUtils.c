#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- private ---
int getNumberOfLines(FILE* file) {
    int lines = 0;

    while(feof(file)) {
        char character = fgetc(file);
        if(character == '\n') {
            lines++;
        }
    }

    return lines;
}

char* getField(char *line, int num) {
    const char *token;
    for(token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n")) {
        if(!--num) {
            return token;
        }
    }

    return NULL; // field on specified line wasn't found.
}

// --- public ---
Student* convertFileToUsers() {
    FILE *file = fopen("Data.csv", "r");
    Student *students = NULL;
    int structSize = sizeof(Student);

    int lines = getNumberOfLines(file);
    students = malloc((lines-2) * structSize); // - 2 because the first two lines aren't anything we need to read.

    int currentLineNum;
    for(currentLineNum = 0; currentLineNum < lines; currentLineNum++) {
        size_t length;
        char* currentLine = fgetln(file, &length);
        Student s;

        s.id = atoi(getField(currentLine, 0));
        s.firstName = getField(currentLine, 1);
        s.lastName = getField(currentLine, 2);
        s.quiz1 = atoi(getField(currentLine, 3));
        s.quiz2 = atoi(getField(currentLine, 4));
        s.quiz3 = atoi(getField(currentLine, 5));
        s.lab1 = atoi(getField(currentLine, 6));
        s.lab2 = atoi(getField(currentLine, 7));
        s.assignment1 = atoi(getField(currentLine, 8));
        s.exam1 = atoi(getField(currentLine, 9));
        s.exam2 = atoi(getField(currentLine, 10));

        students[currentLineNum] = s;
    }

    fclose(file);

    //debug
    printf("s.id = %d", students[0].id);
}
