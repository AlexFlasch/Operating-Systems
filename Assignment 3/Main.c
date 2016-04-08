#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    int arrivalTime;
    int burstTime;
    int turnaround;
    double normTurnaround;
    int latency;
    int startTime;
    int finishTime;
    int priority;
    int name;
    int amountDone;
    int done;
} Process;

typedef struct {
    double fifoTurnaround;
    double fifoNormTurnaround;
    double fifoLatency;
    double fifoUtilization;
    double sjfTurnaround;
    double sjfNormTurnaround;
    double sjfLatency;
    double sjfUtilization;
    double rrTurnaround;
    double rrNormTurnaround;
    double rrLatency;
    double rrUtilization;
    double mfqTurnaround;
    double mfqNormTurnaround;
    double mfqLatency;
    double mfqUtilization;
} Results;

Results r;

int numProcesses;

Process* generateProcesses(int num);
int generateRandomIntInRange(int min, int max);
int processesAreDone(Process *processes);
void runFifo(Process *processes);
void runSjf(Process *processes);
void runRoundRobin(Process *processes);
void runMfq(Process *processes);
Process getHighestPriorityProcess(Process *processes);
int compareArrivalTime(Process *s1, Process *s2);
int compareBurstTime(Process *s1, Process *s2);
void outputToCsv();
void printProcessInfo(Process p);

int main(int argc, char *argv[]) {
    if(argc == 1) {
        numProcesses = 50;
    }
    else if(argv[1] != NULL){
        numProcesses = atoi(argv[1]);
        if(numProcesses > 100) {
            numProcesses = 100;
        }
    }

    Process* processes = generateProcesses(numProcesses);

    // int i;
    // for(i = 0; i < numProcesses; i++) {
    //     Process p = processes[i];
    //     printProcessInfo(p);
    // }

    runFifo(processes);
    printf("===== finished FIFO =====\n\n");
    runSjf(processes);
    printf("===== finished SJF =====\n\n");
    runRoundRobin(processes);
    printf("===== finished Round Robin =====\n\n");
    runMfq(processes);
    printf("===== finished MFQ =====\n\n");

    outputToCsv();
    printf("Results output to results.csv\n");
}

Process* generateProcesses(int num) {
    Process *processes = malloc(sizeof(Process) * num);

    int i;
    for(i = 0; i < num; i++) {
        Process p;
        p.arrivalTime = generateRandomIntInRange(0, 10);
        p.burstTime = generateRandomIntInRange(3, 30);
        p.name = i;
        p.done = 0;
        p.startTime = 0;
        p.finishTime = 0;
        p.priority = 1;
        p.turnaround = 0;
        p.normTurnaround = 0;
        p.latency = 0;
        p.amountDone = 0;

        processes[i] = p;
    }

    return processes;
}

int generateRandomIntInRange(int min, int max) {
    int x = rand() % (max + 1 - min) + min;

    return x;
}

int processesAreDone(Process *processes) {
    int finished = TRUE;

    int i;
    for(i = 0; i < numProcesses; i++) {
        if(!processes[i].done) {
            finished = FALSE;
            break;
        }
    }

    // printf("processes are done: %d\n", finished);
    return finished;
}

void runFifo(Process *processes) {
    int currentTime = 0;
    int busyTime = 0;
    Process *fifoProcesses = malloc(sizeof(processes));

    int i;
    for(i = 0; i < numProcesses; i++) {
        fifoProcesses[i] = processes[i];
    }

    qsort(fifoProcesses, numProcesses, sizeof(Process), (int (*) (const void *, const void*)) compareArrivalTime);

    // run processes
    for(i = 0; i < numProcesses; i++) {
        Process p = fifoProcesses[i];
        p.startTime = currentTime;
        p.latency = currentTime - p.arrivalTime;
        if(p.latency < 0)
            p.latency = 0;

        while(currentTime <= (p.burstTime + p.startTime)) {
            // process hasn't arrived yet
            if(p.arrivalTime >= currentTime) {
                currentTime++;
            }
            // process is running
            else {
                currentTime++;
                busyTime++;
            }
        }

        // printProcessInfo(p);
        // printf("currentTime: %d\nbusyTime: %d\n", currentTime, busyTime);

        p.done = TRUE;
        p.finishTime = currentTime;
        p.turnaround = p.latency + p.burstTime;
        p.normTurnaround = (double) (p.turnaround / p.burstTime);

        // printProcessInfo(p);

        fifoProcesses[i] = p;
    }

    double avgTurnaroundTime = 0;
    double avgNormTurnaroundTime = 0;
    double avgLatency = 0;
    double utilization = (double) busyTime / (double) currentTime;

    // calculate avg values
    for(i = 0; i < numProcesses; i++) {
        Process p = fifoProcesses[i];

        avgTurnaroundTime += p.turnaround;
        avgNormTurnaroundTime += p.normTurnaround;
        avgLatency += p.latency;
    }

    avgTurnaroundTime /= numProcesses;
    avgNormTurnaroundTime /= numProcesses;
    avgLatency /= numProcesses;

    r.fifoTurnaround = avgTurnaroundTime;
    r.fifoNormTurnaround = avgNormTurnaroundTime;
    r.fifoLatency = avgLatency;
    r.fifoUtilization = utilization;

    printf("----- FIFO stats -----\n");
    printf("Average Turnaround: %f\n", r.fifoTurnaround);
    printf("Average Normalized Turnaround: %f\n", r.fifoNormTurnaround);
    printf("Average Latency: %f\n", r.fifoLatency);
    printf("Utilization: %f\n", r.fifoUtilization);
}

void runSjf(Process *processes) {
    int currentTime = 0;
    int busyTime = 0;
    Process *sjfProcesses = malloc(sizeof(processes));

    int i;
    for(i = 0; i < numProcesses; i++) {
        sjfProcesses[i] = processes[i];
    }

    qsort(sjfProcesses, numProcesses, sizeof(Process), (int (*) (const void *, const void *)) compareBurstTime);

    // run processes
    for(i = 0; i < numProcesses; i++) {
        Process p = sjfProcesses[i];
        p.startTime = currentTime;
        p.latency = currentTime - p.arrivalTime;
        if(p.latency < 0)
            p.latency = 0;

        while(currentTime <= (p.burstTime + p.startTime)) {
            // process hasn't arrived yet
            if(p.arrivalTime >= currentTime) {
                currentTime++;
            }
            // process is running
            else {
                currentTime++;
                busyTime++;
            }
        }

        // printProcessInfo(p);
        // printf("currentTime: %d\nbusyTime: %d\n", currentTime, busyTime);

        p.done = TRUE;
        p.finishTime = currentTime;
        p.turnaround = p.latency + p.burstTime;
        p.normTurnaround = (double) (p.turnaround / p.burstTime);

        // printProcessInfo(p);

        sjfProcesses[i] = p;
    }

    double avgTurnaroundTime = 0;
    double avgNormTurnaroundTime = 0;
    double avgLatency = 0;
    double utilization = (double) busyTime / (double) currentTime;

    // calculate avg values
    for(i = 0; i < numProcesses; i++) {
        Process p = sjfProcesses[i];

        avgTurnaroundTime += p.turnaround;
        avgNormTurnaroundTime += p.normTurnaround;
        avgLatency += p.latency;
    }

    avgTurnaroundTime /= numProcesses;
    avgNormTurnaroundTime /= numProcesses;
    avgLatency /= numProcesses;

    r.sjfTurnaround = avgTurnaroundTime;
    r.sjfNormTurnaround = avgNormTurnaroundTime;
    r.sjfLatency = avgLatency;
    r.sjfUtilization = utilization;

    printf("----- SJF stats -----\n");
    printf("Average Turnaround: %f\n", r.sjfTurnaround);
    printf("Average Normalized Turnaround: %f\n", r.sjfNormTurnaround);
    printf("Average Latency: %f\n", r.sjfLatency);
    printf("Utilization: %f\n", r.sjfUtilization);
}

void runRoundRobin(Process *processes) {
    int currentTime = 0;
    int busyTime = 0;
    int quantum = generateRandomIntInRange(1, 10);
    int currentQuantum = 0;
    Process *rrProcesses = malloc(sizeof(Process) * numProcesses);

    int i;
    for(i = 0; i < numProcesses; i++) {
        rrProcesses[i] = processes[i];
    }

    // run processes
    for(i = 0; !processesAreDone(rrProcesses); i++) {
        Process p = rrProcesses[i % numProcesses];
        if(i <= numProcesses) {
            p.startTime = currentTime;
            p.latency = currentTime - p.arrivalTime;
            if(p.latency < 0)
                p.latency = 0;
        }

        while(!p.done || currentQuantum < quantum) {
            // check to see if current process has finished
            if(p.amountDone >= p.burstTime) {
                p.done = TRUE;
                p.finishTime = currentTime;
                p.turnaround = p.latency + p.burstTime;
                p.normTurnaround = (double) (p.turnaround / p.burstTime);

                rrProcesses[p.name] = p;
            }

            // process hasn't arrived yet
            if(p.arrivalTime >= currentTime) {
                currentTime++;
                break;
            }
            // process is running
            else {
                currentTime++;
                busyTime++;
                p.amountDone++;
            }
            currentQuantum++;
        }

        currentQuantum = 0;

        // printProcessInfo(p);
        // printf("currentTime: %d\nbusyTime: %d\n", currentTime, busyTime);
    }

    double avgTurnaroundTime = 0;
    double avgNormTurnaroundTime = 0;
    double avgLatency = 0;
    double utilization = (double) busyTime / (double) currentTime;

    for(i = 0; i < numProcesses; i++) {
        Process p = rrProcesses[i];

        avgTurnaroundTime += p.turnaround;
        avgNormTurnaroundTime += p.normTurnaround;
        avgLatency += p.latency;
    }

    r.rrTurnaround = avgTurnaroundTime;
    r.rrNormTurnaround = avgNormTurnaroundTime;
    r.rrLatency = avgLatency;
    r.rrUtilization = utilization;

    printf("----- Round Robin stats -----\n");
    printf("Average Turnaround: %f\n", r.rrTurnaround);
    printf("Average Normalized Turnaround: %f\n", r.rrNormTurnaround);
    printf("Average Latency: %f\n", r.rrLatency);
    printf("Utilization: %f\n", r.rrUtilization);
}

void runMfq(Process *processes) {
    int currentTime = 0;
    int busyTime = 0;
    int prio1Quantum = 1;
    int prio2Quantum = 2;
    int prio3Quantum = 4;
    int prio4Quantum = 8;
    int randomResetTime = generateRandomIntInRange(100, 500);
    int currentQuantum = 0;
    Process *mfqProcesses = malloc(sizeof(Process) * numProcesses);

    int i;
    for(i = 0; i < numProcesses; i++) {
        mfqProcesses[i] = processes[i];
        // printProcessInfo(mfqProcesses[i]);
    }

    // run processes
    for(i = 0; !processesAreDone(mfqProcesses); i++) {
        Process p = getHighestPriorityProcess(mfqProcesses);
        // printProcessInfo(p);

        // if the process hasn't arrived yet, ignore it and go to the next one
        if(p.arrivalTime > currentTime) {
            // printf("skipping\n");
            currentTime++;
            continue;
        }

        if(i <= numProcesses) {
            p.startTime = currentTime;
            p.latency = currentTime - p.arrivalTime;
            if(p.latency < 0)
                p.latency = 0;
        }

        // eventually reset all processes to priority 1 to avoid starvation
        if(currentTime >= randomResetTime) {
            int j;
            for(j = 0; j < numProcesses; j++) {
                mfqProcesses[j].priority = 1;
            }

            // printf("============ RESET ALL PROCESSES TO PRIO 1 ============\n");
        }

        if(p.priority == 1) {
            // printf("running prio 1 process.\n");
            while(!p.done && currentQuantum < prio1Quantum) {
                // check to see if current process has finished
                if(p.amountDone >= p.burstTime) {
                    p.done = TRUE;
                    p.finishTime = currentTime;
                    p.turnaround = p.latency + p.burstTime;
                    p.normTurnaround = (double) (p.turnaround / p.burstTime);
                }

                // process hasn't arrived yet
                if(p.arrivalTime >= currentTime) {
                    currentTime++;
                    break; // go to next process since it hasn't arrived
                }
                // process is running
                else {
                    currentTime++;
                    busyTime++;
                    p.amountDone++;
                }
                currentQuantum++;
            }

            // if process hasn't finished in prio 1, bump to next prio
            if(!p.done) {
                // printf("bumping process to prio 2\n");
                p.priority = 2;
            }

            currentQuantum = 0;
        }
        else if(p.priority == 2) {
            // printf("running prio 2 process.\n");
            while(!p.done && currentQuantum < prio2Quantum) {
                // check to see if current process has finished
                if(p.amountDone >= p.burstTime) {
                    p.done = TRUE;
                    p.finishTime = currentTime;
                    p.turnaround = p.latency + p.burstTime;
                    p.normTurnaround = (double) (p.turnaround / p.burstTime);
                }

                // process hasn't arrived yet
                if(p.arrivalTime >= currentTime) {
                    break; // go to next process since it hasn't arrived
                }
                // process is running
                else {
                    currentTime++;
                    busyTime++;
                    p.amountDone++;
                }
                currentQuantum++;
            }

            // if process hasn't finished in prio 1, bump to next prio
            if(!p.done) {
                // printf("bumping process to prio 3\n");
                p.priority = 3;
            }

            currentQuantum = 0;
        }
        else if(p.priority == 3) {
            // printf("running prio 3 process.\n");
            while(!p.done && currentQuantum < prio3Quantum) {
                // check to see if current process has finished
                if(p.amountDone >= p.burstTime) {
                    // printf("Process finished.\n");
                    p.done = TRUE;
                    p.finishTime = currentTime;
                    p.turnaround = p.latency + p.burstTime;
                    p.normTurnaround = (double) (p.turnaround / p.burstTime);
                }

                // process hasn't arrived yet
                if(p.arrivalTime >= currentTime) {
                    // printf("skipping process.\n");
                    currentTime++;
                    break; // go to next process since it hasn't arrived
                }
                // process is running
                else {
                    currentTime++;
                    busyTime++;
                    p.amountDone++;
                    // printf("amountDone: %d\n", p.amountDone);
                }
                currentQuantum++;
            }

            // if process hasn't finished in prio 1, bump to next prio
            if(!p.done) {
                // printf("bumping process to prio 4\n");
                p.priority = 4;
            }

            currentQuantum = 0;
        }
        else {
            // printf("running prio 4 process.\n");
            while(!p.done && currentQuantum < prio4Quantum) {
                // check to see if current process has finished
                if(p.amountDone >= p.burstTime) {
                    p.done = TRUE;
                    p.finishTime = currentTime;
                    p.turnaround = p.latency + p.burstTime;
                    p.normTurnaround = (double) (p.turnaround / p.burstTime);
                }

                // process hasn't arrived yet
                if(p.arrivalTime >= currentTime) {
                    currentTime++;
                    break; // go to next process since it hasn't arrived
                }
                // process is running
                else {
                    currentTime++;
                    busyTime++;
                    p.amountDone++;
                }
                currentQuantum++;
            }

            currentQuantum = 0;
        }

        mfqProcesses[p.name] = p;
        // printProcessInfo(mfqProcesses[p.name]);
    }

    double avgTurnaroundTime = 0;
    double avgNormTurnaroundTime = 0;
    double avgLatency = 0;
    double utilization = (double) busyTime / (double) currentTime;

    for(i = 0; i < numProcesses; i++) {
        Process p = mfqProcesses[i];

        avgTurnaroundTime += p.turnaround;
        avgNormTurnaroundTime += p.normTurnaround;
        avgLatency += p.latency;
    }

    r.mfqTurnaround = avgTurnaroundTime;
    r.mfqNormTurnaround = avgNormTurnaroundTime;
    r.mfqLatency = avgLatency;
    r.mfqUtilization = utilization;

    printf("----- MFQ stats -----\n");
    printf("Average Turnaround: %f\n", r.mfqTurnaround);
    printf("Average Normalized Turnaround: %f\n", r.mfqNormTurnaround);
    printf("Average Latency: %f\n", r.mfqLatency);
    printf("Utilization: %f\n", r.mfqLatency);
}

Process getHighestPriorityProcess(Process *processes) {
    int i;
    for(i = 0; i < numProcesses; i++) {
        Process p = processes[i];

        // if the process is done, ignore it, check for others
        if(p.done) {
            continue;
        }

        if(p.priority == 1) {
            // printf("highest process: 1\n");
            return p;
        }
    }
    // nothing in prio 1, search for prio 2
    for(i = 0; i < numProcesses; i++) {
        Process p = processes[i];

        if(p.done) {
            continue;
        }

        if(p.priority == 2) {
            // printf("highest process: 2\n");
            return p;
        }
    }
    // nothing in prio 2, search for prio 3
    for(i = 0; i < numProcesses; i++) {
        Process p = processes[i];

        if(p.done) {
            continue;
        }

        if(p.priority == 3) {
            // printf("highest process: 3\n");
            return p;
        }
    }
    // nothing in prio 3, search for prio 4
    for(i = 0; i < numProcesses; i++) {
        Process p = processes[i];

        if(p.done) {
            continue;
        }

        if(p.priority == 4) {
            // printf("highest process: 4\n");
            return p;
        }
    }
}

int compareArrivalTime(Process *s1, Process *s2) {
    if(s1->arrivalTime < s2->arrivalTime)
        return -1;
    else if(s1->arrivalTime > s2->arrivalTime)
        return 1;
    else return 0;
}

int compareBurstTime(Process *s1, Process *s2) {
    if(s1->burstTime < s2->burstTime)
        return -1;
    else if(s1->burstTime > s2->burstTime)
        return 1;
    else return 0;
}

void outputToCsv() {
    FILE* fileStream = fopen("results.csv", "w+");

    fputs("\t,FIFO,SJF,RR,MFQ\n", fileStream);
    fprintf(fileStream, "Avg Turnaround,%.2f,%.2f,%.2f,%.2f\n", r.fifoTurnaround, r.sjfTurnaround, r.rrTurnaround, r.mfqTurnaround);
    fprintf(fileStream, "Avg Normal Turnaround,%.2f,%.2f,%.2f,%.2f\n", r.fifoNormTurnaround, r.sjfNormTurnaround, r.rrNormTurnaround, r.mfqNormTurnaround);
    fprintf(fileStream, "Avg Latency,%.2f,%.2f,%.2f,%.2f\n", r.fifoLatency, r.sjfLatency, r.rrLatency, r.mfqLatency);
    fprintf(fileStream, "Avg Utilization,%.2f,%.2f,%.2f,%.2f\n", r.fifoUtilization, r.sjfUtilization, r.rrUtilization, r.mfqUtilization);

    fclose(fileStream);
}

// for debugging purposes
void printProcessInfo(Process p) {
    printf("arrivalTime: %d\n", p.arrivalTime);
    printf("burstTime: %d\n", p.burstTime);
    printf("turnaround: %d\n", p.turnaround);
    printf("normTurnaround: %f\n", p.normTurnaround);
    printf("amountDone: %d\n", p.amountDone);
    printf("latency: %d\n", p.latency);
    printf("finishTime: %d\n", p.finishTime);
    printf("priority: %d\n", p.priority);
    printf("name: %d\n", p.name);
    printf("done: %d\n\n", p.done);
}
