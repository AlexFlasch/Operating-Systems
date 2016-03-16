#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

int numOfStudents;
int numOfChairs;
int numOfVisits;

int currAvailChairs;
int chairIsAvailable;

int tutorIsBusy;
int chairsAreBusy;
pthread_cond_t tutorCondition;
pthread_cond_t chairCondition;
pthread_mutex_t tutorLock;
pthread_mutex_t chairLock;

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

void simulateStudent(void *student);

int main(int argc, char *argv[]) {
    // set up execution parameters
    numOfStudents = atoi(argv[1]);
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
    pthread_cond_init(&chairCondition, NULL);
    pthread_mutex_init(&tutorLock, NULL);
    pthread_mutex_init(&chairLock, NULL);
    tutorIsBusy = FALSE;
    chairsAreBusy = FALSE;

    // begin execution

    int i;

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

        printf("Starting simulation of student %d.\n", s.number);

        s.threadId = pthread_create(&students[i].thread, NULL, (void *) &simulateStudent, (void *) s.number);
    }

    for(i = 0; i < numOfStudents; i++) {
        Student s = students[studentSize * i];

        pthread_join(s.thread, NULL);

        printf("Student %d has finished their assignment.\n", s.number);
    }
}

void simulateStudent(void *studentId) {
    int studentSize = sizeof(Student);
    Student s = students[studentSize * (int) studentId];

    // printf("Starting simulation of student %d.\n", s.number);

    while(s.sessionsCompleted < numOfVisits) {
        int sleepTime = rand() % 5;

        printf("Student %d is working on their assignment.\n", s.number);
        sleep(sleepTime);
        printf("Student %d needs help on their assignment.\n", s.number);

        printf("Current available chairs = %d\n", currAvailChairs);
        if((numOfChairs - currAvailChairs) >= 0 && currAvailChairs > 0) {
            while(chairsAreBusy) {
                pthread_cond_wait(&chairCondition, &chairLock);
            }

            printf("Student %d is taking a seat.\n", s.number);

            pthread_mutex_lock(&chairLock);
            chairsAreBusy = TRUE;
            currAvailChairs--;
            pthread_mutex_unlock(&chairLock);
            chairsAreBusy = FALSE;
            pthread_cond_broadcast(&chairCondition);

            while(tutorIsBusy) {
                pthread_cond_wait(&tutorCondition, &tutorLock);
                printf("Student %d is still waiting on the tutor.\n", s.number);
            }

            pthread_mutex_lock(&tutorLock);
            tutorIsBusy = TRUE;

            while(chairsAreBusy) {
                pthread_cond_wait(&chairCondition, &chairLock);
            }

            pthread_mutex_lock(&chairLock);
            chairsAreBusy = TRUE;
            currAvailChairs++;
            pthread_mutex_unlock(&chairLock);
            chairsAreBusy = FALSE;
            pthread_cond_broadcast(&chairCondition);

            // getting help...
            printf("Student %d is getting help.\n", s.number);
            sleep(sleepTime);

            pthread_mutex_unlock(&tutorLock);
            tutorIsBusy = FALSE;

            pthread_cond_broadcast(&tutorCondition);

            s.sessionsCompleted++;

            printf("Student %d has gotten help %d times.\n", s.number, s.sessionsCompleted);
        }
        else {
            printf("Student %d was unable to find a seat.\n", s.number);
        }
    }

    pthread_exit(0);
}
