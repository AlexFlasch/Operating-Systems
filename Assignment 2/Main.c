#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define CHAIR_AVAILABLE (numOfChairs - currAvailChairs) > 0

int numOfStudents;
int numOfChairs;
int numOfVisits;

int currAvailChairs;
int chairIsAvailable;

int tutorIsBusy;
pthread_cond_t tutorCondition;
pthread_cond_t studentCondition;
pthread_mutex_t tutorLock;

int tutorThreadId;
pthread_t tutorThread;

// structure defining a student
// if a students chairPosition = -1 they don't have a chair.
typedef struct {
    int chairPosition;
    int sessionsCompleted;
    int needsHelp;
    int threadId;
    int number;
    pthread_t thread;
} Student;

Student *students;

typedef struct {
    int position;
    int occupied;

} Chair;

Chair *chairs;

void simulateStudent(void *student);

int main(int argc, char *argv[]) {
    // set up execution parameters
    numOfStudents = atoi(argv[1]);
    printf("argc = %d\n", argc);
    if(argc > 2) {
        numOfChairs = atoi(argv[2]);
    }
    else {
        numOfChairs = 3;
    }
    currAvailChairs = numOfChairs;

    if(argc > 3) {
        numOfVisits = atoi(argv[3]);
    }
    else {
        numOfVisits = 3;
    }

    pthread_cond_init(&tutorCondition, NULL);
    pthread_cond_init(&studentCondition, NULL);
    pthread_mutex_init(&tutorLock, NULL);
    tutorIsBusy = FALSE;

    // begin execution

    // create chairs
    int chairSize = sizeof(Chair);
    chairs = malloc(chairSize * numOfChairs);

    int i;
    for(i = 0; i < numOfChairs; i++) {
        Chair *c = malloc(chairSize);

        c->position = i;
        c->occupied = FALSE;

        chairs[chairSize * i] = *c;
    }

    // create students
    int studentSize = sizeof(Student);
    students = malloc(studentSize * numOfStudents);

    for(i = 0; i < numOfStudents; i++) {
        Student *s = malloc(studentSize);

        s->chairPosition = -1;
        s->sessionsCompleted = 0;
        s->needsHelp = FALSE;
        s->number = i;

        students[studentSize * i] = *s;
    }

    // tutorThreadId = pthread_create(&tutorThread, NULL, (void *) &simulateTutor, NULL);

    // create student threads
    for(i = 0; i < numOfStudents; i++) {
        Student s = students[studentSize * i];

        s.threadId = pthread_create(&students[i].thread, NULL, (void *) &simulateStudent, &s);
    }
}

void simulateStudent(void *student) {
    Student *s = (Student *) student;

    printf("Starting simulation of student %d.\n", s->number);

    while(s->sessionsCompleted < numOfVisits) {
        int sleepTime = rand() / 10;

        printf("Student %d is working on their assignment.\n", s->number);
        sleep(sleepTime);
        printf("Student %d needs help on their assignment.\n", s->number);

        if((numOfChairs - currAvailChairs) > 0) {
            currAvailChairs--;

            printf("Student %d is taking a seat.\n", s->number);

            while(tutorIsBusy) {
                pthread_cond_wait(&tutorCondition, &tutorLock);
            }

            pthread_mutex_lock(&tutorLock);
            tutorIsBusy = TRUE;
            currAvailChairs++;

            // getting help...
            printf("Student %d is getting help.\n", s->number);
            sleep(sleepTime);

            pthread_mutex_unlock(&tutorLock);
            tutorIsBusy = FALSE;

            pthread_cond_broadcast(&tutorCondition);

            s->sessionsCompleted++;
        }
        else {
            printf("Student %d was unable to find a seat.\n", s->number);
        }
    }

    printf("Student %d has finished their assignment.\n", s->number);
}
