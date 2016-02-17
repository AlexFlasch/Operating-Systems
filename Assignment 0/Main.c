#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Student *students;
int length;

double getOverallGradeForStudent(Student s);

int* getMinGrades(Student *s);

int* getMaxGrades(Student *s);

double* getAvgGrades(Student *s);

double* getStandardDeviations(Student *s, double *avgs);

void printGradingScheme(Student *students);

int main(int argc, char *argv[]) {
    // get array of structs from read file
    students = convertFileToUsers(argv[1]);

    length = studentsLength();

    // calculate all required user data
    int *minGrades = getMinGrades(students);
    int *maxGrades = getMaxGrades(students);
    double *avgGrades = getAvgGrades(students);
    double *stdDeviations = getStandardDeviations(students, avgGrades);

    // output student data
    FILE *output = fopen(argv[2], "w");
    fprintf(output, "%6s, %10s, %9s, %5s, %5s, %5s, %4s, %5s, %11s, %5s, %5s, %5s\n",
        "Id", "First Name", "Last Name", "Quiz1", "Quiz2", "Quiz3", "Lab1", "Lab2",
        "Assignment1", "Exam1", "Exam2", "Grade");
    int i;
    for(i = 0; i < length; i++) {
        Student s = students[i * sizeof(Student)];
        fprintf(output, "%6d, %10s, %9s, %5d, %5d, %5d, %4d, %5d, %11d, %5d, %5d, %5.2f\n",
            s.id, s.firstName, s.lastName, s.quiz1, s.quiz2, s.quiz3, s.lab1, s.lab2,
            s.assignment1, s.exam1, s.exam2, s.finalGrade);
    }
    fprintf(output, "%6s, %28d, %5d, %5d, %4d, %5d, %11d, %5d, %5d, %5.2f\n", "Min",
        minGrades[0], minGrades[1], minGrades[2], minGrades[3], minGrades[4],
        minGrades[5], minGrades[6], minGrades[7], minGrades[8]);
    fprintf(output, "%6s, %28d, %5d, %5d, %4d, %5d, %11d, %5d, %5d, %5.2f\n", "Max",
        maxGrades[0], maxGrades[1], maxGrades[2], maxGrades[3], maxGrades[4],
        maxGrades[5], maxGrades[6], maxGrades[7], maxGrades[8]);
    fprintf(output, "%6s, %28.2f, %5.2f, %5.2f, %4.2f, %5.2f, %11.2f, %5.2f, %5.2f, %5.2f\n", "Avg",
        avgGrades[0], avgGrades[1], avgGrades[2], avgGrades[3], avgGrades[4],
        avgGrades[5], avgGrades[6], avgGrades[7], avgGrades[8]);
    fprintf(output, "%14s, %20.2f, %5.2f, %5.2f, %4.2f, %5.2f, %11.2f, %5.2f, %5.2f, %5.2f\n", "Std. Deviation",
        stdDeviations[0], stdDeviations[1], stdDeviations[2], stdDeviations[3], stdDeviations[4],
        stdDeviations[5], stdDeviations[6], stdDeviations[7], stdDeviations[8]);
}

int* getMinGrades(Student *s) {
    int *lowestGrades = malloc(sizeof(int) * 9);

    // start with high values so it gets overwritten
    int lowestQuiz1 = 300;
    int lowestQuiz2 = 300;
    int lowestQuiz3 = 300;
    int lowestLab1 = 300;
    int lowestLab2 = 300;
    int lowestAssignment1 = 300;
    int lowestExam1 = 300;
    int lowestExam2 = 300;
    double lowestFinalGrade = 300;

    int j;
    // get lowest quiz1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz1 < lowestQuiz1)
            lowestQuiz1 = s[j * sizeof(Student)].quiz1;
    }
    lowestGrades[0] = lowestQuiz1;

    // get lowest quiz2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz2 < lowestQuiz2)
            lowestQuiz2 = s[j * sizeof(Student)].quiz2;
    }
    lowestGrades[1] = lowestQuiz2;

    // get lowest quiz3 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz3 < lowestQuiz3)
            lowestQuiz3 = s[j * sizeof(Student)].quiz3;
    }
    lowestGrades[2] = lowestQuiz3;

    // get lowest lab1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].lab1 < lowestLab1)
            lowestLab1 = s[j * sizeof(Student)].lab1;
    }
    lowestGrades[3] = lowestLab1;

    // get lowest lab2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].lab2 < lowestLab2)
            lowestLab2 = s[j * sizeof(Student)].lab2;
    }
    lowestGrades[4] = lowestLab2;

    // get lowest assignment1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].assignment1 < lowestAssignment1)
            lowestAssignment1 = s[j * sizeof(Student)].assignment1;
    }
    lowestGrades[5] = lowestAssignment1;

    // get lowest exam1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].exam1 < lowestExam1)
            lowestExam1 = s[j * sizeof(Student)].exam1;
    }
    lowestGrades[6] = lowestExam1;

    // get lowest exam2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].exam2 < lowestExam2)
            lowestExam2 = s[j * sizeof(Student)].exam2;
    }
    lowestGrades[7] = lowestExam2;

    // get lowest overall grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].finalGrade < lowestFinalGrade)
            lowestFinalGrade = s[j * sizeof(Student)].finalGrade;
    }
    lowestGrades[8] = lowestFinalGrade;

    return lowestGrades;
}

int* getMaxGrades(Student *s) {
    int *highestGrades = malloc(sizeof(int) * 9);

    // start with low values so it gets overwritten
    int highestQuiz1 = 0;
    int highestQuiz2 = 0;
    int highestQuiz3 = 0;
    int highestLab1 = 0;
    int highestLab2 = 0;
    int highestAssignment1 = 0;
    int highestExam1 = 0;
    int highestExam2 = 0;
    double highestFinalGrade = 0;

    int j;
    // get highest quiz1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz1 > highestQuiz1)
            highestQuiz1 = s[j * sizeof(Student)].quiz1;
    }
    highestGrades[0] = highestQuiz1;

    // get highest quiz2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz2 > highestQuiz2)
            highestQuiz2 = s[j * sizeof(Student)].quiz2;
    }
    highestGrades[1] = highestQuiz2;

    // get highest quiz3 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].quiz3 > highestQuiz3)
            highestQuiz3 = s[j * sizeof(Student)].quiz3;
    }
    highestGrades[2] = highestQuiz3;

    // get highest lab1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].lab1 > highestLab1)
            highestLab1 = s[j * sizeof(Student)].lab1;
    }
    highestGrades[3] = highestLab1;

    // get highest lab2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].lab2 > highestLab2)
            highestLab2 = s[j * sizeof(Student)].lab2;
    }
    highestGrades[4] = highestLab2;

    // get highest assignment1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].assignment1 > highestAssignment1)
            highestAssignment1 = s[j * sizeof(Student)].assignment1;
    }
    highestGrades[5] = highestAssignment1;

    // get highest exam1 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].exam1 > highestExam1)
            highestExam1 = s[j * sizeof(Student)].exam1;
    }
    highestGrades[6] = highestExam1;

    // get highest exam2 grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].exam2 > highestExam2)
            highestExam2 = s[j * sizeof(Student)].exam2;
    }
    highestGrades[7] = highestExam2;

    // get highest final grade
    for(j = 0; j < length; j++) {
        if(s[j * sizeof(Student)].finalGrade > highestFinalGrade) {
            highestGrades[8] = s[j * sizeof(Student)].finalGrade;
        }
    }
    highestGrades[8] = highestFinalGrade;

    return highestGrades;
}

double* getAvgGrades(Student *s) {
    double *avgGrades = malloc(sizeof(double) * 9);

    int j;

    // get avg quiz1 grade
    double avgQuiz1;
    double sumQuiz1 = 0;

    for(j = 0; j < length; j++) {
        sumQuiz1 += s[j * sizeof(Student)].quiz1;
    }

    avgQuiz1 = sumQuiz1/length;
    avgGrades[0] = avgQuiz1;

    // get avg quiz2 grade
    double avgQuiz2;
    double sumQuiz2 = 0;

    for(j = 0; j < length; j++) {
        sumQuiz2 += s[j * sizeof(Student)].quiz2;
    }

    avgQuiz2 = sumQuiz2/length;
    avgGrades[1] = avgQuiz2;

    // get avg quiz3 grade
    double avgQuiz3;
    double sumQuiz3 = 0;

    for(j = 0; j < length; j++) {
        sumQuiz3 += s[j * sizeof(Student)].quiz3;
    }

    avgQuiz3 = sumQuiz3/length;
    avgGrades[2] = avgQuiz3;

    // get avg lab1 grade
    double avgLab1;
    double sumLab1 = 0;

    for(j = 0; j < length; j++) {
        sumLab1 += s[j * sizeof(Student)].lab1;
    }

    avgLab1 = sumLab1/length;
    avgGrades[3] = avgLab1;

    // get avg lab2 grade
    double avgLab2;
    double sumLab2 = 0;

    for(j = 0; j < length; j++) {
        sumLab2 += s[j * sizeof(Student)].lab2;
    }

    avgLab2 = sumLab2/length;
    avgGrades[4] = avgLab2;

    // get avg assignment1 grade
    double avgAssignment1;
    double sumAssignment1 = 0;

    for(j = 0; j < length; j++) {
        sumAssignment1 += s[j * sizeof(Student)].assignment1;
    }

    avgAssignment1 = sumAssignment1/length;
    avgGrades[5] = avgAssignment1;

    // get avg exam1 grade
    double avgExam1;
    double sumExam1 = 0;

    for(j = 0; j < length; j++) {
        sumExam1 += s[j * sizeof(Student)].exam1;
    }

    avgExam1 = sumExam1/length;
    avgGrades[6] = avgExam1;

    // get avg exam2 grade
    double avgExam2;
    double sumExam2 = 0;

    for(j = 0; j < length; j++) {
        sumExam2 += s[j * sizeof(Student)].exam2;
    }

    avgExam2 = sumExam2/length;
    avgGrades[7] = avgExam2;

    // get avg final grade
    double avgFinal;
    double sumFinal = 0;

    for(j = 0; j < length; j++) {
        sumFinal += s[j * sizeof(Student)].finalGrade;
    }

    avgFinal = sumFinal/length;
    avgGrades[8] = avgFinal;

    return avgGrades;
}

double* getStandardDeviations(Student *s, double *avgs) {
    double *stdDeviations = malloc(sizeof(double) * 9);

    // quiz 3
    double numerator = 0;

    int j;
    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].quiz1 - avgs[0]), 2.0);
    }

    double denominator = length - 1;

    double fraction = (numerator / denominator);

    double stdDeviation = sqrt(fraction);

    stdDeviations[0] = stdDeviation;

    // quiz 2
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].quiz2 - avgs[1]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[1] = stdDeviation;

    // quiz 3
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].quiz3 - avgs[2]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[2] = stdDeviation;

    // lab 1
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].lab1 - avgs[3]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[3] = stdDeviation;

    // lab 2
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].lab2 - avgs[4]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[4] = stdDeviation;

    // assignment 1
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].assignment1 - avgs[5]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[5] = stdDeviation;

    // exam 1
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].exam1 - avgs[6]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[6] = stdDeviation;

    // exam 2
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].exam2 - avgs[7]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[7] = stdDeviation;

    // final grade
    numerator = 0;

    for(j = 0; j < length; j++) {
        numerator += pow(((double) s[j * sizeof(Student)].finalGrade - avgs[8]), 2.0);
    }

    fraction = (numerator / denominator);

    stdDeviation = sqrt(fraction);

    stdDeviations[8] = stdDeviation;

    return stdDeviations;
}
