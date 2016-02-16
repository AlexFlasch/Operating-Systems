// protect against multiple includes.
#ifndef FileUtils
#define FileUtils

typedef struct {
    int id;
    char *firstName;
    char *lastName;
    int quiz1;
    int quiz2;
    int quiz3;
    int lab1;
    int lab2;
    int assignment1;
    int exam1;
    int exam2;
    int finalGrade;
} Student;

Student* convertFileToUsers();

#endif
