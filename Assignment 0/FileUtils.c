#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numOfStudents;

// --- private ---
int getNumberOfLines(char *input) {
    FILE *file = fopen(input, "r");
    int lines = 0;

    char character = getc(file);
    while(character != EOF) {
        if(character == '\n') {
            lines++;
        }
        character = getc(file);
    }

    fclose(file);
    return lines;
}

char* getField(char *line, int num) {
    char *token;
    char l[256];

    strcpy(l, line);

    int tokenNum = 0;
    token = strtok(l, ",");
    while(tokenNum != num) {
        token = strtok(NULL, ",");
        tokenNum++;
    }

    return token;
}

double getOverallGradeForStudent(Student s) {
    double quizPercent = 0.1;
    double labPercent = 0.2;
    double assignmentPercent = 0.3;
    double examPercent = 0.4;

    // get quiz percent
    int quizTotalPoints = 30;
    int quizAchievedPoints = s.quiz1 + s.quiz2 + s.quiz3;
    double quizScore = ((double) quizAchievedPoints)/((double) quizTotalPoints);

    // get lab percent
    int labTotalPoints = 40;
    int labAchievedPoints = s.lab1 + s.lab2;
    double labScore = ((double) labAchievedPoints)/((double) labTotalPoints);

    // assignment score
    int assignmentAchievedPoints = s.assignment1;
    double assignmentScore = ((double) assignmentAchievedPoints)/((double) 50);

    // get exam score
    int examTotalPoints = 200;
    int examAchievedPoints = s.exam1 + s.exam2;
    double examScore = ((double) examAchievedPoints)/((double) examTotalPoints);

    double overallGrade = 100 * ((quizScore * quizPercent) + (labScore * labPercent)
        + (assignmentScore * assignmentPercent) + (examScore * examPercent));

    return overallGrade;
}

// --- public ---
Student* convertFileToUsers(char *input) {
    FILE *file = fopen(input, "r");
    Student* students;
    int structSize = sizeof(Student);

    int lines = getNumberOfLines(input);
    students = malloc((lines-2) * structSize); // - 2 because the first two lines aren't anything we need to read.

    char currentLine[256];
    fgets(currentLine, 256, file);
    fgets(currentLine, 256, file);
    int currentLineNum;
    for(currentLineNum = 0; currentLineNum < lines - 2; currentLineNum++) {
        fgets(currentLine, 256, file);
        Student *s = malloc(structSize);

        s->id = atoi(getField(currentLine, 0));
        s->firstName = strdup(getField(currentLine, 1));
        s->lastName = strdup(getField(currentLine, 2));
        s->quiz1 = atoi(getField(currentLine, 3));
        s->quiz2 = atoi(getField(currentLine, 4));
        s->quiz3 = atoi(getField(currentLine, 5));
        s->lab1 = atoi(getField(currentLine, 6));
        s->lab2 = atoi(getField(currentLine, 7));
        s->assignment1 = atoi(getField(currentLine, 8));
        s->exam1 = atoi(getField(currentLine, 9));
        s->exam2 = atoi(getField(currentLine, 10));
        s->finalGrade = getOverallGradeForStudent(*s);

        students[currentLineNum * structSize] = *s;
    }

    fclose(file);

    numOfStudents = currentLineNum;
    return students;
}

int studentsLength() {
    return numOfStudents;
}
